#include <cub/mem/ObjectAllocator.h>
#include <cub/mem/Placement.h>
#include <foo/l4-sched/FooEvent.h>
#include <foo/l4-sched/FooStateId.h>
#include <foo/l4-sched/FooUnstableStateInfoRepo.h>
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
        struct Info
                : UnstableStateInfo
                , cub::Singleton<Info>
        {
            OVERRIDE(StateId getStateId() const)
            {
                return STATE::getStateId();
            }

            OVERRIDE(const TransStrategyDecisionMaker* getStrategyMaker() const)
            {
                return STATE::getStrategyDecisionMaker();
            }

            OVERRIDE(UnstableState* create(tsl::InstanceId iid))
            {
                return new FooUnstableState<STATE>(iid);
            }
        };

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
#define __S(id, type) case id: return __STATE(type)::Info::getInstance()

///////////////////////////////////////////////////////////////////
UnstableStateInfo& FooUnstableStateInfoRepo::getUnstableInfoByEvent(ev::EventId eventId)
{
    switch(eventId)
    {
    __S(EV_EVENT1_T,  FooTrans1State);
    __S(EV_EVENT4_T,  FooTrans2State);
    __S(EV_EVENT_R,   FooReleaseState);
    }

    return NilUnStableStateInfo::getInstance();
}

L4_NS_END
