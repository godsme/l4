#ifndef H1F4190E4_D7C1_41A1_9AD7_24B3627B903A
#define H1F4190E4_D7C1_41A1_9AD7_24B3627B903A

#include <state/l4.h>
#include <cub/repo/list/ListElem.h>
#include <event/concept/EventId.h>
#include <event/concept/EventInfo.h>
#include <state/TransactionEvent.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

struct TransStrategyDecisionMaker;
struct FailedRequestListener;

struct BufferedEventInfo : cub::ListElem<BufferedEventInfo>
                         , ev::EventInfo
                         , TransactionEvent
{
   BufferedEventInfo(const TransStrategyDecisionMaker* strategy, const FailedRequestListener* listener);

   OVERRIDE(ev::EventId getEventId() const);
   OVERRIDE(cub::Status updateEventId(const ev::EventId) const);

   void enQueue();
   void deQueue();

   bool isInQueue() const;

public:
   OVERRIDE(void reject(const tsl::InstanceId iid) const);
   OVERRIDE(const EventInfo& getEventInfo() const);
   OVERRIDE(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const);

private:
   const TransStrategyDecisionMaker*  strategy;
   const FailedRequestListener* listener;
   mutable ev::EventId eventId;
   bool inQueue;

public:
   ABSTRACT(cub::Status saveEvent(const EventInfo&));

   ABSTRACT(const void* getMsg() const);
   ABSTRACT(const void* getMsgBuffer() const);
   ABSTRACT(size_t getMsgSize() const);
};

L4_NS_END

#endif /* H1F4190E4_D7C1_41A1_9AD7_24B3627B903A */
