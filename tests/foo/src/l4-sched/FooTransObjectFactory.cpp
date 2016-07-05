#include <foo/l4-sched/FooTransObjectFactory.h>
#include <cub/mem/Placement.h>
#include <foo/l4-sched/FooStateId.h>
#include <foo/l4-sched/state/unstable/FooReleaseState.h>
#include <foo/l4-sched/state/unstable/FooTrans1State.h>
#include <foo/l4-sched/state/unstable/FooTrans2State.h>

L4_NS_BEGIN

using namespace cub;

namespace
{
    union
    {
        Placement<FooTrans1State>  trans1;
        Placement<FooTrans2State>  trans2;
        Placement<FooReleaseState> release;
    }u;
}

FooTrans1State a(0);

///////////////////////////////////////////////////////////////////
UnstableState* FooTransObjectFactory::createState(const tsl::InstanceId iid, const StateId sid)
{
    switch(sid)
    {
    case STATE_Trans1:
        return new(&u) FooTrans1State(iid);
    case STATE_Trans2:
        return new(&u) FooTrans2State(iid);
    case STATE_Release:
        return new(&u) FooReleaseState(iid);
    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////
void FooTransObjectFactory::destroyState(UnstableState* state)
{
    state->~UnstableState();
}

L4_NS_END
