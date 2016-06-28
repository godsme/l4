#ifndef HA5940673_4F58_43C3_84AE_1AF0FD82DBD2
#define HA5940673_4F58_43C3_84AE_1AF0FD82DBD2

#include <state/l4.h>
#include <state/TransStrategyDecisionMaker.h>
#include <event/event.h>
#include <trans-dsl/sched/concept/InstanceId.h>

FWD_DECL_EV(EventInfo);

L4_NS_BEGIN

struct TransactionEvent : TransStrategyDecisionMaker
{
   ABSTRACT(const ev::EventInfo& getEventInfo() const);
   ABSTRACT(void reject(const tsl::InstanceId iid) const);
};

L4_NS_END

#endif /* HA5940673_4F58_43C3_84AE_1AF0FD82DBD2 */
