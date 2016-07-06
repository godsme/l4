#include <state/TransStateMachine.h>
#include <cub/base/Assertions.h>
#include <state/State.h>
#include <state/FailedRequestListener.h>
#include <state/EventQueue.h>
#include <state/PostProcessHandler.h>
#include <state/TransAvailMutexFetcher.h>

#include <event/impl/ScatteredEventInfo.h>
#include <event/impl/ConsecutiveEventInfo.h>
#include <event/impl/SimpleEventInfo.h>
#include <event/concept/Event.h>

#include <cub/gof/Singleton.h>
#include <cub/base/Likely.h>
#include <state/BufferedEventInfo.h>
#include <state/StateFactory.h>

#include <trans-dsl/ext/mutex/def/TransMutexAvailMsg.h>
#include <trans-dsl/utils/ActionStatus.h>
#include <trans-dsl/ext/signal/concept/TransSignalId.h>
#include <trans-dsl/ext/signal/action/TransSignalMsg.h>
#include <trans-dsl/TslStatus.h>

#include <cub/log/log.h>

L4_NS_BEGIN

using namespace cub;
using namespace tsl;
using namespace ev;

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::leaveState(const Status cause)
{
   if(__unlikely(state == 0)) return TSL_SUCCESS;
   return state->leave(*this, cause);
}

//////////////////////////////////////////////////////////////
inline TransStrategy TransStateMachine::getStrategyOnState(const Event& event) const
{
    //if(!factory.isStrategyEvent(event.getEventId())) return TS_NIL;

    if(__unlikely(state == 0)) return TS_NIL;

    return state->getStrategy(iid, event);
}

//////////////////////////////////////////////////////////////
inline bool TransStateMachine::stateIgnoreFailure() const
{
   return state == 0 ? false : state->ignoreFailure();
}

//////////////////////////////////////////////////////////////
inline void TransStateMachine::quitState(Status cause)
{
   if(__unlikely(state == 0)) return;

   return state->kill(*this, cause);
}

//////////////////////////////////////////////////////////////
TransStateMachine::TransStateMachine(StateFactory& factory, EventQueue& eventQueue, tsl::InstanceId iid)
   : state(0)
   , suspendState(0)
   , nextStateId(STATE_NIL)
   , factory(factory)
   , eventQueue(eventQueue)
   , iid(iid)
{}

//////////////////////////////////////////////////////////////
TransStateMachine::~TransStateMachine()
{
   state = 0;
}

//////////////////////////////////////////////////////////////
InstanceId TransStateMachine::getInstanceId() const
{
   return iid;
}

//////////////////////////////////////////////////////////////
Status TransStateMachine::start()
{
   return enterState(factory.createInitialState());
}

//////////////////////////////////////////////////////////////
void TransStateMachine::transitTo(const StateId stateId)
{
   nextStateId = stateId;
}

///////////////////////////////////////////////////////////////////
inline StateId TransStateMachine::getStateId() const
{
    return state == nullptr ? STATE_NIL : state->getId();
}

//////////////////////////////////////////////////////////////
inline StateId TransStateMachine::fetchNextState()
{
   StateId stateId = nextStateId;
   nextStateId = STATE_NIL;
   return stateId;
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::run(State* newState, const Event& event)
{
   ActionStatus status = enterState(newState);
   if(!status.isWorking())
   {
       resetInitState();
       return status;
   }

   return handleEventOnState(event);
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::startTransaction(const Event& event)
{
    quiteSuspendState(TSL_FORCE_STOPPED);// XXX: it's only allowed to create a non-concurrent state for dcm ng
    return run(factory.createUnstableState(event.getEventId()), event);
}


//////////////////////////////////////////////////////////////
inline Status TransStateMachine::startFailTransaction(const Status cause, const Event& event)
{
   ActionStatus status = run(factory.createFailState(cause, event), event);
   if(status.isDone())
   {
       return gotoNextState();
   }

   return status;
}

//////////////////////////////////////////////////////////////////////////
inline void TransStateMachine::quiteSuspendState(const Status cause)
{
    if(__likely(suspendState == 0)) return;

    suspendState->kill(*this, cause);

    suspendState = 0;
}

///////////////////////////////////////////////////////////////////
inline Status TransStateMachine::reject(const Event& event)
{
    FailedRequestListener* listener = factory.getFailedRequestListener(event.getEventId());
    if(listener == nullptr)
    {
        return TSL_UNKNOWN_EVENT;
    }

    listener->onReject(iid, event, getStateId());

    event.consume();

    return TSL_CONTINUE;
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::doBuffer(const Event& event)
{
    //CUB_ASSERT_TRUE(isTransEvent(event));

    auto strategy = factory.getStrategyMaker(event.getEventId());
    if(strategy == nullptr)
    {
        return TSL_UNKNOWN_EVENT;
    }

    auto listener = factory.getFailedRequestListener(event.getEventId());

    return eventQueue.put(iid, event, *strategy, listener);
}

//////////////////////////////////////////////////////////////
inline void TransStateMachine::discardBufferedEvent(EventId eventId)
{
    EventInfo* event = eventQueue.discardBufferedEvent(eventId);
    if(event != nullptr)
    {
        discard(*event);
        delete event;
    }
}

//void TransStateMachine::doDiscard(EventId eventId)
//{
//    //CUB_ASSERT_TRUE_VOID(isTransEvent(Event(SimpleEventInfo(eventId))));
//
//    discardBufferedEvent(eventId);
//}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::buffer(const Event& event)
{
    ActionStatus status = doBuffer(event);
    if(status.isDone())
    {
        return TSL_CONTINUE;
    }

    return discard(event);
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::purge(const Event& event)
{
    //CUB_ASSERT_TRUE(isTransEvent(event));

    // Failed request listener is allowed to be null.
    const TransStrategyDecisionMaker* strategy = factory.getStrategyMaker(event.getEventId());
    if(strategy == nullptr)
    {
        return TSL_UNKNOWN_EVENT;
    }

    FailedRequestListener* listener = factory.getFailedRequestListener(event.getEventId());

    return eventQueue.purge(iid, event, *strategy, listener);
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::preempt(const Event& event)
{
    //CUB_ASSERT_TRUE(isTransEvent(event));

    //
    // IMPORTANT:
    //
    // if this event could preempt the running state, it's quite
    // likely that it could preempt the buffered events as well.
    // so purge the event queue to discard or reject buffered events.
    //
    CUB_ASSERT_SUCC_CALL(purge(event));

    ActionStatus status = leaveState(factory.getPreemptCauseByEvent(event));
    if(status.isDone())
    {
        return startTransaction(event);
    }

    //
    // This event is buffered, only if the running transaction cannot be stopped immediately.
    //
    if(status.isWorking())
    {
        return buffer(event);
    }

    return status;
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::replace(const Event& event)
{
 //   CUB_ASSERT_TRUE(isTransEvent(event));

    quitState(factory.getInterruptCauseByEvent(event));

    return startTransaction(event);
}

//////////////////////////////////////////////////////////////
inline void TransStateMachine::rejectAllBufferedEvents()
{
    while(EventInfo* event = eventQueue.getFirstEvent())
    {
        reject(*event);
        delete event;
    }
}

//////////////////////////////////////////////////////////////////////////
inline void TransStateMachine::rejectAllEvents(const Event& event)
{
    //if(!event.isConsumed() && !factory.isTerminalEvent(event.getEventId()))
    if(!event.isConsumed())
    {
        reject(event);
    }

    rejectAllBufferedEvents();
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::onFail(const Status cause, const Event& event)
{
    rejectAllEvents(event);

    if(stateIgnoreFailure())
    {
        return TSL_CONTINUE;
    }

    quitState(cause);
    quiteSuspendState(cause);

   return startFailTransaction(cause, event);
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::rej_fail(const Event& event)
{
   ActionStatus status = reject(event);
   if(__unlikely(status.isFailed()))
   {
      return status;
   }

   return fail(event);
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::rej_interrupt(const Event& event)
{
    ActionStatus status = reject(event);
    if(__unlikely(status.isFailed()))
    {
        return status;
    }

    return leaveState(factory.getInterruptCauseByEvent(event));
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::interrupt(const Event& event)
{
   event.consume();
   return leaveState(factory.getInterruptCauseByEvent(event));
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::interrupt_fail(const Event& event)
{
    event.consume();
    leaveState(factory.getInterruptCauseByEvent(event));

    return factory.getFailCauseByEvent(event);
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::fail(const Event& event)
{
   return factory.getFailCauseByEvent(event);
}

//////////////////////////////////////////////////////////////
Status TransStateMachine::discard(const Event& event)
{
   FailedRequestListener* listener = factory.getFailedRequestListener(event.getEventId());
   if(listener == nullptr)
   {
       return TSL_UNKNOWN_EVENT;
   }

   listener->onDiscard(iid, event, getStateId());

   event.consume();

   return TSL_CONTINUE;
}

//////////////////////////////////////////////////////////////
Status TransStateMachine::interrupt_suspend(const Event& event)
{
    CUB_ASSERT_TRUE(suspendState == 0);

    ActionStatus status = leaveState(factory.getPreemptCauseByEvent(event));
    if(__unlikely(status.isFailed()))
    {
        return status;
    }

    if(status.isWorking())
    {
        suspendState = state;
    }
    else
    {
        factory.destroyState(state);
    }

    return run(factory.createPriUnstableState(event.getEventId()), event);
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::suspend(const Event& event)
{
    CUB_ASSERT_TRUE(suspendState == 0);

    suspendState = state;

    return run(factory.createPriUnstableState(event.getEventId()), event);
}

////////////////////////////////////////////////////////////////////////////
//inline bool TransStateMachine::isTransEvent(const Event& event) const
//{
//    return factory.isTransEvent(event.getEventId());
//}

//////////////////////////////////////////////////////////////
Status TransStateMachine::handleTransEvent(const Event& event)
{
   switch(getStrategyOnState(event))
   {
   case TS_REPLACE:           return replace(event);
   case TS_REJECT:            return reject(event);
   case TS_BUFFER:            return buffer(event);
   case TS_PREEMPT:           return preempt(event);
   case TS_FAIL:              return fail(event);
   case TS_REJ_FAIL:          return rej_fail(event);
   case TS_REJ_INTERRUPT:     return rej_interrupt(event);
   case TS_INTERRUPT:         return interrupt(event);
   case TS_INTERRUPT_FAIL:    return interrupt_fail(event);
   case TS_DISCARD:           return discard(event);
   case TS_SUSPEND:           return suspend(event);
   case TS_INTERRUPT_SUSPEND: return interrupt_suspend(event);
   case TS_NIL:
   default:                   return TSL_UNKNOWN_EVENT;
   }
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::enterState(State* newState)
{
    CUB_ASSERT_VALID_PTR_R(newState, TSL_FATAL_BUG);

    state = newState;

    return state->enter(*this);
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::popUpSuspendState()
{
    if(__likely(suspendState == 0)) return TSL_SUCCESS;

    factory.destroyState(state);
    state = suspendState;
    suspendState = 0;

    return TSL_CONTINUE;
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::gotoNewState()
{
    ActionStatus status = TSL_SUCCESS;

    while(status.isDone())
    {
        status = enterState(factory.createStableState(fetchNextState()));
    }

    return status;
}

//////////////////////////////////////////////////////////////
template <typename EVENT_HANDLER>
inline Status TransStateMachine::dispatchEvent(const Event& event)
{
   ActionStatus status = EVENT_HANDLER::handleEvent(*this, event);
   if(!status.isDone())
   {
      return status;
   }

   return onStateDone(event);
}

//////////////////////////////////////////////////////////////////////////
struct BufferedEventHandler
{
    static Status handleEvent(TransStateMachine& sm, const Event& event)
    {
        return sm.handleTransEvent(event);
    }
};

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::processBufferedEvent(BufferedEventInfo* event)
{
    Status status = processEvent<BufferedEventHandler>(*event);

    //
    // IMPORTANT:
    //
    // For the sake of performance & reduction of failure risk of
    // buffered event allocation, this buffered event might be
    // re-buffered in event queue again. In this case, it should
    // NOT be deleted.
    //
    if(!event->isInQueue())
    {
        delete event;
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////
Status TransStateMachine::doReescheduleOnUnstable()
{
    ActionStatus status = TSL_CONTINUE;

    while(status.isWorking())
    {
        BufferedEventInfo* event = eventQueue.getFirstEvent();
        if(event == 0)
        {
            return TSL_CONTINUE;
        }

        status = processBufferedEvent(event);
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::rescheduleOnUnstable()
{
    CUB_ASSERT_SUCC_CALL(eventQueue.startReschedule());

    Status status = doReescheduleOnUnstable();

    eventQueue.reescheduleDone(iid);

    return status;
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::gotoNextState()
{
    ActionStatus status = popUpSuspendState();
    if(status.isWorking())
    {
        return rescheduleOnUnstable();
    }

    return gotoNewState();
}

/////////////////////////////////////////////////////////////
inline Status TransStateMachine::onStateDone(const Event& event)
{
    if(!event.isConsumed())
    {
        buffer(event);
    }

    return gotoNextState();
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::handleEventOnState(const Event& event)
{
    CUB_ASSERT_VALID_PTR_R(state, TSL_FATAL_BUG);

    return state->handleEvent(*this, event);
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::handleEventOnSuspendState(const Event& event)
{
    if(__likely(suspendState == 0))
    {
        return TSL_UNKNOWN_EVENT;
    }

    ActionStatus status = suspendState->handleEvent(*this, event);
    if(!status.isWorking())
    {
        factory.destroyState(suspendState);
        suspendState = 0;
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////
namespace
{
    inline bool keepSchedEvent(const ActionStatus status, const Event& event)
    {
        return status.isWorking() && !event.isConsumed();
    }
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::handleEventOnStates(const Event& event)
{
    ActionStatus status = handleEventOnState(event);
    if(!keepSchedEvent(status, event))
    {
        return status;
    }

    status = handleEventOnSuspendState(event);
    if(!keepSchedEvent(status, event))
    {
        return status.isDone() ? TSL_CONTINUE : (Status)status;
    }

    return handleTransEvent(event);
}

//////////////////////////////////////////////////////////////////////////
inline Status TransStateMachine::doDispatchEvent(const Event& event)
{
    ActionStatus status = preState(event);
    if(status.eventAccepted())
    {
        return status;
    }

    status = handleEventOnStates(event);
    if(status.eventAccepted())
    {
        return status;
    }

    return postState(event);
}

//////////////////////////////////////////////////////////////////////////
struct ExternalEventHandler
{
    static Status handleEvent(TransStateMachine& sm, const Event& event)
    {
        return sm.doDispatchEvent(event);
    }
};

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::dispatchNewEvent(const Event& event)
{
    return dispatchEvent<ExternalEventHandler>(event);
}

//////////////////////////////////////////////////////////////
inline Status TransStateMachine::dispatchBufferedEvent(const Event& event)
{
    return dispatchEvent<BufferedEventHandler>(event);
}

//////////////////////////////////////////////////////////////////////////
Status TransStateMachine::dispatchMutexEvent()
{
    TransMutexId mutex;
    while(__unlikely((mutex = SimpleTransAvailMutex::fetchAailMutex()) != 0xFF))
    {
        ActionStatus status = dispatchNewEvent(ConsecutiveEventInfo(EV_MUTEX_UNLOCK, TransMutexAvailMsg(mutex)));
        if(!status.isWorking())
        {
            return status;
        }
    }

    return TSL_CONTINUE;
}

//////////////////////////////////////////////////////////////////////////
Status TransStateMachine::dispatchSignal()
{
    TransSignalId signal;
    while(__unlikely((signal = AbstractTransSignalScheduler::fetchSignal()) != 0xFF))
    {
        ActionStatus status = dispatchNewEvent(ConsecutiveEventInfo(EV_GLOBAL_TRANS_SIGNAL, TransSignalMsg(signal)));
        if(!status.isWorking())
        {
            return status;
        }
    }

    return TSL_CONTINUE;
}

//////////////////////////////////////////////////////////////
Status TransStateMachine::dispatchOthers()
{
    if(__unlikely(availMutex > 0))
    {
        ActionStatus status = dispatchMutexEvent();
        if(!status.isWorking())
        {
            return status;
        }
    }

    if(__unlikely(signalBitmap > 0))
    {
        return dispatchSignal();
    }

    return TSL_CONTINUE;
}

//////////////////////////////////////////////////////////////
template <typename EVENT_HANDLER>
inline Status TransStateMachine::doHandleEvent(const Event& event)
{
    ActionStatus status = dispatchEvent<EVENT_HANDLER>(event);
    if(status != TSL_CONTINUE)
    {
        return status;
    }

    return dispatchOthers();
}

////////////////////////////////////////////////////////////////////////////
template <typename EVENT_HANDLER>
inline Status TransStateMachine::processEvent(const Event& event)
{
    ActionStatus status= doHandleEvent<EVENT_HANDLER>(event);
    if(__unlikely(status.isFailed()))
    {
        return onFail(status, event);
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////
Status TransStateMachine::scheduleIdle()
{
    ActionStatus status = state->onIdleTransSchedule(*this);
    if(__unlikely(status.isFailed()))
    {
        return status;
    }

    if(eventQueue.hasBufferedEvent())
    {
        return TSL_CONTINUE;
    }

    state->onIdle(*this);

    return TSL_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
Status TransStateMachine::scheduleOnStableState()
{
    ActionStatus status = TSL_CONTINUE;

    while(isStable() && status.isWorking())
    {
        BufferedEventInfo* event = eventQueue.getFirstEvent();
        if(event == 0)
        {
            status = scheduleIdle();
            if(status == TSL_SUCCESS)
            {
                return TSL_CONTINUE;
            }
        }
        else
        {
            status = processBufferedEvent(event);
        }
    }

    return status;
}

//////////////////////////////////////////////////////////////
Status TransStateMachine::handleEvent(const Event& event)
{
    Status status = processEvent<ExternalEventHandler>(event);
    if(status == TSL_CONTINUE && isStable())
    {
        // Unknown event should NOT trigger this scheduling.
        return scheduleOnStableState();
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////
bool TransStateMachine::isStable() const
{
    return state ? state->isStable() : false;
}

/////////////////////////////////////////////////////////////////////////
Status TransStateMachine::preState(const Event&)
{
    return TSL_UNKNOWN_EVENT;
}

//////////////////////////////////////////////////////////////////////////
Status TransStateMachine::postState(const Event& event)
{
    return TSL_UNKNOWN_EVENT;
}

//////////////////////////////////////////////////////////////////////////
void TransStateMachine::resetInitState()
{
    if(state != 0 && state->isInitState())
    {
        state->onReentry(*this);
    }
}

/////////////////////////////////////////////////////////////////////////
bool TransStateMachine::isDone() const
{
    return state == nullptr ? true : state->isFinalState();
}

L4_NS_END

