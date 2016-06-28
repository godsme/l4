#ifndef H61811A05_F748_453B_9F32_F4B2302A47B5
#define H61811A05_F748_453B_9F32_F4B2302A47B5

#include <cub/dci/Role.h>
#include <cub/base/Status.h>
#include <state/StateId.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

DEFINE_ROLE(StateMachine)
{
   ABSTRACT(tsl::InstanceId getInstanceId() const);

   // When state should transit, a event handler should
   // call this function to achieve that.
   ABSTRACT(void transitTo(const StateId));
};

L4_NS_END

#endif /* H61811A05_F748_453B_9F32_F4B2302A47B5 */
