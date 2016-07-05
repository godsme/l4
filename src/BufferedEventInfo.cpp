#include <event/concept/Event.h>
#include <state/BufferedEventInfo.h>
#include <state/FailedRequestListener.h>
#include <trans-dsl/TslStatus.h>

L4_NS_BEGIN

using namespace ev;
using namespace cub;
using namespace tsl;

////////////////////////////////////////////////////////////////////////////
BufferedEventInfo::BufferedEventInfo
   ( const TransStrategyDecisionMaker* strategy
   , const FailedRequestListener* listener)
   : strategy(strategy)
   , listener(listener)
   , eventId(0xFFFF)
   , inQueue(false)
{
}

////////////////////////////////////////////////////////////////////////////
EventId BufferedEventInfo::getEventId() const
{
   return eventId;
}

////////////////////////////////////////////////////////////////////////////
Status BufferedEventInfo::updateEventId(const EventId id) const
{
   eventId = id;
   return TSL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////
void BufferedEventInfo::reject(const InstanceId iid) const
{
   if(listener != 0)
   {
      listener->onReject(iid, Event(*this), STATE_NIL);
   }
}

////////////////////////////////////////////////////////////////////////////
TransStrategy BufferedEventInfo::getStrategy(const Event& event) const
{
   return (strategy == 0) ? TS_FAIL : strategy->getStrategy(event);
}

////////////////////////////////////////////////////////////////////////////
const EventInfo& BufferedEventInfo::getEventInfo() const
{
   return *this;
}

//////////////////////////////////////////////////////////////////////////
void BufferedEventInfo::enQueue()
{
    inQueue = true;
}

//////////////////////////////////////////////////////////////////////////
void BufferedEventInfo::deQueue()
{
    inQueue = false;
}

//////////////////////////////////////////////////////////////////////////
bool BufferedEventInfo::isInQueue() const
{
    return inQueue;
}

L4_NS_END
