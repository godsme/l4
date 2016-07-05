#ifndef H5D210E11_52EE_4343_87F1_359A7D41ADF8
#define H5D210E11_52EE_4343_87F1_359A7D41ADF8

#include <state/StableState.h>
#include <foo/l4-sched/FooStateDef.h>
#include <foo/l4-sched/FooStateId.h>

L4_NS_BEGIN

struct FooActiveState : StableState
{
    DEF_STATE_ID(Active);

    OVERRIDE(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const);
};

L4_NS_END

#endif /* H5D210E11_52EE_4343_87F1_359A7D41ADF8 */
