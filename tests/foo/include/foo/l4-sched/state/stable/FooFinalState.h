#ifndef H24D3950B_4734_4B4F_B062_F4A040A24CE2
#define H24D3950B_4734_4B4F_B062_F4A040A24CE2

#include <foo/l4-sched/FooStateDef.h>
#include <state/StableState.h>
#include <foo/l4-sched/FooStateId.h>
#include <state/TransStrategy.h>

L4_NS_BEGIN

struct FooFinalState : StableState
{
    DEF_STATE_ID(Final);

    OVERRIDE(bool isFinalState() const)
    {
        return true;
    }

    OVERRIDE(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const)
    {
        return TS_UNKNOWN;
    }
};

L4_NS_END

#endif /* H24D3950B_4734_4B4F_B062_F4A040A24CE2 */
