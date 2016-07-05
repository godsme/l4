
#include <cub/base/Assertions.h>
#include <state/EventQueue.h>
#include <state/TransactionEvent.h>
#include <event/concept/Event.h>
#include <state/BufferedEventFactory.h>
#include <state/BufferedEventInfo.h>
#include <state/FailedRequestListener.h>
#include <state/TransStrategyDecisionMaker.h>
#include <trans-dsl/TslStatus.h>

L4_NS_BEGIN

using namespace ev;
using namespace cub;
using namespace tsl;

namespace
{
   struct SimpleTransEvent : TransactionEvent
   {
      SimpleTransEvent(const Event& event, TransStrategyDecisionMaker& strategy, FailedRequestListener* listener)
         : event(event), strategy(strategy), listener(listener) {}

      OVERRIDE(TransStrategy getStrategy(const Event& event) const)
      {
         return strategy.getStrategy(event);
      }

      OVERRIDE(const EventInfo& getEventInfo() const)
      {
         return event.getEventInfo();
      }

      OVERRIDE(void reject(const InstanceId iid) const)
      {
         if(listener != 0) listener->onReject(iid, event, STATE_NIL);
      }

   private:
      const Event& event;
      const TransStrategyDecisionMaker& strategy;
      const FailedRequestListener* listener;
   };
}

//////////////////////////////////////////////////////////////////////////////////////
struct EventQueue::Scheduler
{
   Scheduler(const InstanceId iid, EventQueue& queue) :
      iid(iid), queue(queue)
   {
   }

   ~Scheduler()
   {
      LIST_FOREACH(BufferedEventInfo, info, lessPriEvents)
      {
         queue.pushBack(iid, *info);
      }
   }

   Status purge
       ( const Event& event
       , TransStrategyDecisionMaker& strategy
       , FailedRequestListener* listener)
   {
       Status status = doSchedule(SimpleTransEvent(event, strategy, listener));
       if(status == TSL_CONTINUE)
       {
           return TSL_SUCCESS;
       }

       return status;
   }

   Status reschedule
            ( const Event& event
            , TransStrategyDecisionMaker& strategy
            , FailedRequestListener* listener)
   {
        SimpleTransEvent incoming(event, strategy, listener);
        Status status = doSchedule(incoming);
        if (status != TSL_CONTINUE)
        {
            return status;
        }

        status = queue.pushBack(event, strategy, listener);
        if (status != TSL_SUCCESS)
        {
            incoming.reject(iid);
            return status;
        }

        return TSL_SUCCESS;
   }

private:

   void handlePreemptedEvent(const TransactionEvent& event, BufferedEventInfo* preempted)
   {
       TransStrategy strategy = event.getStrategy(preempted->getEventInfo());
      switch(strategy)
      {
      case TS_BUFFER:
         lessPriEvents.pushBack(*preempted);
         break;
      case TS_REJECT:
         preempted->reject(iid);
         delete preempted;
         break;
      case TS_INTERRUPT:
      case TS_INTERRUPT_FAIL:
      case TS_INTERRUPT_SUSPEND:
      case TS_FAIL:
      case TS_REJ_FAIL:
      case TS_REJ_INTERRUPT:
      case TS_REPLACE:
      case TS_SUSPEND:
      case TS_PREEMPT:
      case TS_DISCARD:
      case TS_NIL:
      case TS_UNKNOWN:
      default:
         delete preempted;
         break;
      }
   }

   typedef List<BufferedEventInfo>::Iterator Iterator;

   Status preempt(Iterator& info, const TransactionEvent& event)
   {
      queue.erase(info);
      handlePreemptedEvent(event, &(*info));

      return TSL_CONTINUE;
   }

   Status replace(Iterator& info, const TransactionEvent& incoming)
   {
      queue.erase(info);
      delete &(*info);

      return TSL_CONTINUE;
   }

   Status fail(Iterator&, const TransactionEvent&)
   {
      queue.reset();
      lessPriEvents.reset();

      return TSL_INVALID_DATA;
   }

   Status rej_fail(Iterator& info, const TransactionEvent& event)
   {
      event.reject(iid);
      return fail(info, event);
   }

   Status rej_interrupt(Iterator& info, const TransactionEvent& event)
   {
       event.reject(iid);
       return interrupt(info, event);
   }

   Status suspend(Iterator& info, const TransactionEvent& event)
   {
      return preempt(info, event);
   }

   Status interrupt_suspend(Iterator& info, const TransactionEvent& event)
   {
       interrupt(info, event);
       return TSL_CONTINUE;
   }

   Status reject(Iterator&, const TransactionEvent& event)
   {
      event.reject(iid);
      return TSL_SUCCESS;
   }

   Status interrupt(Iterator& info, const TransactionEvent&)
   {
      queue.erase(info);
      delete &(*info);

      return TSL_UNKNOWN_EVENT;
   }

   Status process(Iterator& info, const TransactionEvent& incomingEvent)
   {
      switch (info->getStrategy(incomingEvent.getEventInfo()))
      {
      case TS_NIL:
      case TS_DISCARD:
      case TS_UNKNOWN:           return TSL_UNKNOWN_EVENT;
      case TS_BUFFER:            return TSL_CONTINUE;
      case TS_PREEMPT:           return preempt(info, incomingEvent);
      case TS_REPLACE:           return replace(info, incomingEvent);
      case TS_REJ_FAIL:          return rej_fail(info, incomingEvent);
      case TS_REJ_INTERRUPT:     return rej_interrupt(info, incomingEvent);
      case TS_FAIL:              return fail(info, incomingEvent);
      case TS_SUSPEND:           return suspend(info, incomingEvent);
      case TS_REJECT:            return reject(info, incomingEvent);
      case TS_INTERRUPT:         return interrupt(info, incomingEvent);
      case TS_INTERRUPT_SUSPEND: return interrupt_suspend(info, incomingEvent);
      case TS_INTERRUPT_FAIL:    return fail(info, incomingEvent);
      default:
         return TSL_INVALID_DATA;
      }
   }

   Status doSchedule(const TransactionEvent& event)
   {
      LIST_FOREACH(BufferedEventInfo, info, queue.getBufferedQueue())
      {
         Status status = process(info, event);
         if(status != TSL_CONTINUE)
         {
            return status;
         }
      }

      return TSL_CONTINUE;
   }

private:
   const InstanceId iid;
   EventQueue& queue;
   List<BufferedEventInfo> lessPriEvents;
};


const U8 MAX_BUFFERED_EVENTS_PER_UE = 3;

//////////////////////////////////////////////////////////////////////////////////////
EventQueue::EventQueue(BufferedEventFactory& factory)
    : factory(factory)
    , current(0)
    , unstable(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////
EventQueue::~EventQueue()
{
   reset();
}

//////////////////////////////////////////////////////////////////////////////////////
void EventQueue::reset()
{
   bufferedEvents[0].reset();
   bufferedEvents[1].reset();
}

//////////////////////////////////////////////////////////////////////////
bool EventQueue::hasBufferedEvent() const
{
    return !bufferedEvents[current].isEmpty();
}

//////////////////////////////////////////////////////////////////////////////////////
Status EventQueue::put
         ( const InstanceId iid
         , const Event& event
         , TransStrategyDecisionMaker& strategy
         , FailedRequestListener* listener)
{
   Status status = Scheduler(iid, *this).reschedule(event, strategy, listener);
   event.consume();
   return status;
}

//////////////////////////////////////////////////////////////////////////
Status EventQueue::purge
          ( const InstanceId iid
          , const Event& event
          , TransStrategyDecisionMaker& strategy
          , FailedRequestListener* listener)
{
    return Scheduler(iid, *this).purge(event, strategy, listener);
}


//////////////////////////////////////////////////////////////////////////////////////
BufferedEventInfo* EventQueue::getFirstEvent()
{
    BufferedEventInfo* info = bufferedEvents[current].popFront();
    if(info != 0)
    {
        info->deQueue();
    }

    return info;
}

//////////////////////////////////////////////////////////////////////////
Status EventQueue::enQueue(BufferedEventInfo& info)
{
    CUB_ASSERT_TRUE_R(getBufferedQueue().getSize() < MAX_BUFFERED_EVENTS_PER_UE, TSL_OUT_OF_SCOPE);

    getBufferedQueue().pushBack(info);
    info.enQueue();

    return TSL_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////
void EventQueue::pushBack(const InstanceId iid, BufferedEventInfo& info)
{
   Status status = enQueue(info);
   if(status != TSL_SUCCESS)
   {
      info.reject(iid);
      delete &info;
   }
}

//////////////////////////////////////////////////////////////////////////
bool EventQueue::isBufferedEvent(const Event& event) const
{
    return factory.isBufferedEvent(event.getEventInfo());
}

//////////////////////////////////////////////////////////////////////////
BufferedEventInfo* EventQueue::actualCreateBufferedEvent
        ( const Event& event
         , TransStrategyDecisionMaker& strategy
         , FailedRequestListener* listener)
{
    BufferedEventInfo* info = factory.createBufferedEvent(event, strategy, listener);
    if (info == 0)
    {
        DBG_LOG("actualCreateBufferedEvent::info createBufferedEvent");
        return 0;
    }

    Status status = info->saveEvent(event.getEventInfo());
    if (status != TSL_SUCCESS)
    {
        DBG_LOG("actualCreateBufferedEvent::info saveEvent");
        delete info;
        return 0;
    }

    return info;
}
//////////////////////////////////////////////////////////////////////////////////////
BufferedEventInfo* EventQueue::createBufferedEvent
         ( const Event& event
         , TransStrategyDecisionMaker& strategy
         , FailedRequestListener* listener)
{
    if(isBufferedEvent(event))
    {
        return const_cast<BufferedEventInfo*>(&static_cast<const BufferedEventInfo&>(event.getEventInfo()));
    }

    return actualCreateBufferedEvent(event, strategy, listener);
}

//////////////////////////////////////////////////////////////////////////////////////
void EventQueue::erase(Iterator& info)
{
   getBufferedQueue().erase(info);
   info->deQueue();
}

//////////////////////////////////////////////////////////////////////////
inline U8 EventQueue::getBufferedQueueIndex()
{
    return unstable ? !current : current;
}

//////////////////////////////////////////////////////////////////////////
inline List<BufferedEventInfo>& EventQueue::getBufferedQueue()
{
    return bufferedEvents[getBufferedQueueIndex()];
}

BufferedEventInfo* EventQueue::discardBufferedEvent(EventId eventId)
{
    LIST_FOREACH(BufferedEventInfo, info, getBufferedQueue())
    {
        if(info->getEventId() == eventId)
        {
            erase(info);

            return info.getValue();
        }
    }

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////
Status EventQueue::pushBack(const Event& event, TransStrategyDecisionMaker& strategy,
         FailedRequestListener* listener)
{
   BufferedEventInfo* info = createBufferedEvent(event, strategy, listener);
   CUB_ASSERT_VALID_PTR(info);

   Status status = enQueue(*info);
   if(status != TSL_SUCCESS)
   {
      if(!isBufferedEvent(event))
      {
          delete info;
      }
   }

   return status;
}

//////////////////////////////////////////////////////////////////////////
Status EventQueue::startReschedule()
{
    CUB_ASSERT_FALSE(unstable);

    unstable = 1;

    bufferedEvents[!current].reset();

    return TSL_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
Status EventQueue::reescheduleDone(const InstanceId iid)
{
    if(!unstable) return TSL_SUCCESS;

    LIST_FOREACH(BufferedEventInfo, info, bufferedEvents[current])
    {
        pushBack(iid, *info);
    }

    current = !current;
    unstable = 0;

    return TSL_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
void EventQueue::dump(const char* info)
{
    if(getBufferedQueue().isEmpty())
        return;

    U8 index = 0;
    LIST_FOREACH(BufferedEventInfo, event, getBufferedQueue())
    {
        index++;
    }
}

L4_NS_END
