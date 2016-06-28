#ifndef HC9EB1236_E137_44B3_BC4D_41BDDE7C2790
#define HC9EB1236_E137_44B3_BC4D_41BDDE7C2790

#include <cub/dci/Role.h>
#include <event/event.h>
#include <state/StateId.h>
#include <trans-dsl/sched/concept/InstanceId.h>


FWD_DECL_EV(Event);

L4_NS_BEGIN

DEFINE_ROLE(PostProcessHandler)
{
   DEFAULT(void, onPostProcess(const tsl::InstanceId iid, const ev::Event& event, const StateId sid) const);
};

L4_NS_END

#endif /* HC9EB1236_E137_44B3_BC4D_41BDDE7C2790 */
