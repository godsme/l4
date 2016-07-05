#include <cub/mem/ObjectAllocator.h>
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
    template <typename STATE>
    struct FooUnstableState
            : STATE
    {
        explicit FooUnstableState(tsl::InstanceId iid)
            : STATE(iid)
        {}


        void* operator new(size_t size) throw();
        void operator delete(void* p);
    };

    ///////////////////////////////////////////////////////////////////
    #define __STATE(type) FooUnstableState<type>
    #define PLACE(type) Placement<__STATE(type) >  obj##type

    union Block
    {
        PLACE(FooTrans1State);
        PLACE(FooTrans2State);
        PLACE(FooReleaseState);
    };

    cub::ObjectAllocator<Block, 500> allocator;


    template <typename STATE>
    void* FooUnstableState<STATE>::operator new(size_t size) throw()
    {
        if(size > sizeof(Block)) return nullptr;

        return allocator.alloc();
    }

    template <typename STATE>
    void FooUnstableState<STATE>::operator delete(void* p)
    {
        allocator.free(p);
    }
}

///////////////////////////////////////////////////////////////////
#define __S(id, type) case STATE_ ## id: return new __STATE(type)(iid)

///////////////////////////////////////////////////////////////////
UnstableState* FooTransObjectFactory::createState(const tsl::InstanceId iid, const StateId sid)
{
    switch(sid)
    {
    __S(Trans1,  FooTrans1State);
    __S(Trans2,  FooTrans2State);
    __S(Release, FooReleaseState);
    }

    return nullptr;
}

L4_NS_END
