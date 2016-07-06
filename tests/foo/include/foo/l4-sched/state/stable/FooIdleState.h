#ifndef HF5FA8408_FAA8_4A85_BFF5_9837275242E7
#define HF5FA8408_FAA8_4A85_BFF5_9837275242E7

#include <foo/l4-sched/FooStateDef.h>
#include <state/StableState.h>
#include <foo/l4-sched/FooStateId.h>

L4_NS_BEGIN

struct FooIdleState : StableState
{
    DEF_STATE_ID(Idle);

    OVERRIDE(bool isInitState() const)
    {
        return true;
    }

    OVERRIDE(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const);
};

L4_NS_END

#endif /* HF5FA8408_FAA8_4A85_BFF5_9837275242E7 */
