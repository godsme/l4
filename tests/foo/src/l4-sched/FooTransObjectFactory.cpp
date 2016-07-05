#include <foo/l4-sched/FooTransObjectFactory.h>
#include <cub/mem/Placement.h>
#include <foo/l4-sched/FooStateId.h>
#include <foo/l4-sched/FooTrans1State.h>

L4_NS_BEGIN

using namespace cub;

namespace
{
    union
    {
        Placement<FooTrans1State> trans1;
    }u;
}

FooTrans1State a(0);

///////////////////////////////////////////////////////////////////
UnstableState* FooTransObjectFactory::createState(const tsl::InstanceId iid, const StateId sid)
{
//    switch(sid)
//    {
//    case STATE_Trans1:
//       // return u.trans1.alloc();
//    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////
void FooTransObjectFactory::destroyState(UnstableState* state)
{

}

L4_NS_END
