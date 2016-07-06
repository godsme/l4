#ifndef HE87F0F8D_0B9A_48B5_B61C_CA31908BE2C6
#define HE87F0F8D_0B9A_48B5_B61C_CA31908BE2C6

#include <cub/dci/Role.h>
#include <cub/gof/Singleton.h>
#include <state/StateId.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

DEFINE_ROLE(UnstableStateInfo)
{
    ABSTRACT(StateId getStateId() const);
    ABSTRACT(const TransStrategyDecisionMaker* getStrategyMaker() const);
    ABSTRACT(UnstableState* create(tsl::InstanceId));
};

///////////////////////////////////////////////////////////////////
struct NilUnStableStateInfo : UnstableStateInfo, cub::Singleton<NilUnStableStateInfo>
{
    OVERRIDE(StateId getStateId() const) { return STATE_NIL; }

    OVERRIDE(const TransStrategyDecisionMaker* getStrategyMaker() const)
    {
        return nullptr;
    }

    OVERRIDE(UnstableState* create(tsl::InstanceId))
    {
        return nullptr;
    }
};

L4_NS_END

#endif /* HE87F0F8D_0B9A_48B5_B61C_CA31908BE2C6 */
