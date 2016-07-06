#ifndef H9CF3A86F_1051_41CA_9423_BC9525E15E8B
#define H9CF3A86F_1051_41CA_9423_BC9525E15E8B

#include <event/concept/EventId.h>
#include <state/l4.h>
#include <event/event.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <cub/repo/list/List.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

struct UeId;
struct FailedRequestListener;
struct BufferedEventFactory;
struct TransStrategyDecisionMaker;
struct BufferedEventInfo;

struct EventQueue
{
   explicit EventQueue(BufferedEventFactory& factory);
   virtual ~EventQueue();

   bool hasBufferedEvent() const;

   cub::Status startReschedule();
   cub::Status reescheduleDone(const tsl::InstanceId iid);

   cub::Status put(const tsl::InstanceId iid
             , const ev::Event&
             , const TransStrategyDecisionMaker&
             , FailedRequestListener*);

   cub::Status purge
       ( const tsl::InstanceId
       , const ev::Event&
       , const TransStrategyDecisionMaker&
       , FailedRequestListener*);

   BufferedEventInfo* getFirstEvent();
   BufferedEventInfo* discardBufferedEvent(ev::EventId eventId);

   void reset();
   void dump(const char* info = 0);

private:
   cub::Status createBufferedEvent(const ev::Event& event, BufferedEventInfo& bufferedInfo);
   void pushBack(const tsl::InstanceId iid, BufferedEventInfo& info);
   cub::Status pushBack
            ( const ev::Event& event
            , const TransStrategyDecisionMaker& strategy
            , FailedRequestListener* listener);
   BufferedEventInfo* createBufferedEvent
            ( const ev::Event& event
            , const TransStrategyDecisionMaker& strategy
            , FailedRequestListener* listener);

   BufferedEventInfo* actualCreateBufferedEvent
       ( const ev::Event& event
       , const TransStrategyDecisionMaker& strategy
       , FailedRequestListener* listener);

   cub::Status enQueue(BufferedEventInfo&);

   bool isBufferedEvent(const ev::Event&) const;

private:
   typedef cub::List<BufferedEventInfo>::Iterator Iterator;

private:
   void erase(Iterator& info);
   cub::List<BufferedEventInfo>& getBufferedQueue();
   cub::U8 getBufferedQueueIndex();

   struct Scheduler;

private:
   BufferedEventFactory& factory;
   cub::List<BufferedEventInfo> bufferedEvents[2];
   cub::U8 current:1;
   cub::U8 unstable:1;
};

L4_NS_END

#endif /* H9CF3A86F_1051_41CA_9423_BC9525E15E8B */
