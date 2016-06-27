#ifndef H61811A05_F748_453B_9F32_F4B2302A47B5
#define H61811A05_F748_453B_9F32_F4B2302A47B5

#include <sched/l4.h>
#include <cub/dci/Role.h>
#include <event/event.h>
#include <cub/base/Status.h>
#include <trans-dsl/sched/concept/InstanceId.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

DEFINE_ROLE(StateMachine)
{
   ABSTRACT(tsl::InstanceId getInstanceId() const);
   ABSTRACT(void transitTo(const StateId));
   ABSTRACT(cub::Status schedEvent(const ev::Event&));
};

L4_NS_END

#endif /* H61811A05_F748_453B_9F32_F4B2302A47B5 */
