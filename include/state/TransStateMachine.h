#ifndef H943BF41F_2128_4C33_B1E5_0988129E5D6C
#define H943BF41F_2128_4C33_B1E5_0988129E5D6C

#include <event/concept/EventId.h>
#include <state/StateMachine.h>
#include <event/event.h>
#include <state/SimpleTransAvailMutex.h>
#include <state/TransMsgDiscarder.h>
#include <state/TransMsgSender.h>
#include <state/TransStrategy.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <trans-dsl/ext/signal/impl/AbstractTransSignalScheduler.h>
#include <trans-dsl/TslStatus.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

struct State;
struct StateFactory;

struct State;
struct EventQueue;
struct BufferedEventInfo;

struct TransStateMachine
    : protected SimpleTransAvailMutex
    , protected tsl::AbstractTransSignalScheduler
    , TransMsgSender
    //, TransMsgDiscarder
    , StateMachine
{
   TransStateMachine
     ( StateFactory&
     , EventQueue&
     , tsl::InstanceId);

   ~TransStateMachine();

   cub::Status start();
   cub::Status handleEvent(const ev::Event& event);

   bool isStable() const;

private:
   OVERRIDE(cub::Status doBuffer(const ev::Event& event));
   //OVERRIDE(void doDiscard(ev::EventId eventId));

private:
   cub::Status handleTransEvent(const ev::Event& event);
   cub::Status discard(const ev::Event&);
   cub::Status reject(const ev::Event& event);
   cub::Status buffer(const ev::Event& event);
   cub::Status preempt(const ev::Event& event);
   cub::Status suspend(const ev::Event& event);
   cub::Status replace(const ev::Event& event);
   cub::Status interrupt(const ev::Event&);
   cub::Status interrupt_fail(const ev::Event&);
   cub::Status interrupt_suspend(const ev::Event& event);
   cub::Status rej_fail(const ev::Event&);
   cub::Status rej_interrupt(const ev::Event& event);
   cub::Status fail(const ev::Event&);
   cub::Status interrupt_replace(const ev::Event&);

   cub::Status onFail(const cub::Status, const ev::Event&);

   cub::Status startTransaction(const ev::Event& event);
   cub::Status startFailTransaction(const cub::Status, const ev::Event& event);

   cub::Status run(State*, const ev::Event&);
   cub::Status onStateDone(const ev::Event&);

   void rejectAllEvents(const ev::Event& event);
   void rejectAllBufferedEvents();
   void discardBufferedEvent(ev::EventId eventId);

   //bool isTransEvent(const ev::Event& event) const;

   cub::Status rescheduleOnUnstable();
   cub::Status doReescheduleOnUnstable();
   cub::Status processBufferedEvent(BufferedEventInfo* event);


public:
   OVERRIDE(void transitTo(const StateId));
   OVERRIDE(tsl::InstanceId getInstanceId() const);


private:
   StateId fetchNextState();
   cub::Status popUpSuspendState();
   cub::Status gotoNextState();
   cub::Status gotoNewState();

   cub::Status onStates(const ev::Event& event);
   cub::Status onStatesSchedule(const ev::Event& event);
   cub::Status scheduleEventOnStates(const ev::Event& event);
   cub::Status purge(const ev::Event& event);
   cub::Status dispatchMutexEvent();
   cub::Status dispatchSignal();

   template <typename EVENT_HANDLER>
   cub::Status doHandleEvent(const ev::Event& event);

   template <typename EVENT_HANDLER>
   cub::Status dispatchEvent(const ev::Event& event);

   template <typename EVENT_HANDLER>
   cub::Status processEvent(const ev::Event&);

   cub::Status doDispatchEvent(const ev::Event& event);
   cub::Status scheduleOnStableState();

   cub::Status dispatchNewEvent(const ev::Event& event);
   cub::Status dispatchBufferedEvent(const ev::Event& event);
   cub::Status scheduleIdle();
   cub::Status dispatchOthers();

   friend struct BufferedEventHandler;
   friend struct ExternalEventHandler;

private:
   cub::Status enterState(State* state);
   cub::Status leaveState(const cub::Status);
   void   quitState(const cub::Status cause = TSL_SUCCESS);
   void quiteSuspendState(const cub::Status cause);
   void resetInitState();

   cub::Status handleEventOnState(const ev::Event& );
   cub::Status handleEventOnStates(const ev::Event&);
   cub::Status handleEventOnSuspendState(const ev::Event&);
   TransStrategy getStrategyOnState(const ev::Event&) const;
   bool stateIgnoreFailure() const;
   StateId getStateId() const;

private:
   State*           state;
   State*           suspendState;
   StateId          nextStateId;

private:
   virtual cub::Status preState(const ev::Event& event);
   virtual cub::Status postState(const ev::Event& event);

   StateFactory& factory;
   EventQueue& eventQueue;
   tsl::InstanceId iid;

};

L4_NS_END

#endif /* H943BF41F_2128_4C33_B1E5_0988129E5D6C */
