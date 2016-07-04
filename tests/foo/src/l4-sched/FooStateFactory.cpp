#include <cub/log/log.h>
#include <foo/l4-sched/FooStateFactory.h>
#include <foo/l4-sched/FooIdleState.h>

#include <cub/mem/Placement.h>
#include <foo/l4-sched/FooEvent.h>
#include <state/StateId.h>
#include <trans-dsl/TslStatus.h>

L4_NS_BEGIN

using namespace cub;

union StableStateSpace
{
    cub::Placement<FooIdleState> idle;
};

///////////////////////////////////////////////////////////////////
FooStateFactory::FooStateFactory()
{
    static_assert(sizeof(u) >= sizeof(StableStateSpace), "");
}

///////////////////////////////////////////////////////////////////
FooStateFactory::~FooStateFactory()
{
}

///////////////////////////////////////////////////////////////////
State* FooStateFactory::createInitialState()
{
    return createStableState(STATE_Idle);
}

///////////////////////////////////////////////////////////////////
State* FooStateFactory::createFailState(const cub::Status, const ev::Event&)
{
    return nullptr;
}

///////////////////////////////////////////////////////////////////
State* FooStateFactory::createStableState(const StateId id)
{
    stateId = id;

    switch (stateId)
    {
    case STATE_Idle:
        return new (&u) FooIdleState;
    case STATE_Active:
    default:
        return nullptr;
    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////
State* FooStateFactory::createUnstableState(const ev::EventId eid)
{
    DBG_LOG("create unstable state : %d", eid);

    return nullptr;
}

///////////////////////////////////////////////////////////////////
State* FooStateFactory::createPriUnstableState(const ev::EventId)
{
    return nullptr;
}

///////////////////////////////////////////////////////////////////
TransStrategyDecisionMaker* FooStateFactory::getStrategyMaker(const ev::EventId eid)
{
    DBG_LOG("get strategy maker : %d", eid);

    return nullptr;
}

///////////////////////////////////////////////////////////////////
FailedRequestListener* FooStateFactory::getFailedRequestListener(const ev::EventId)
{
    return nullptr;
}

///////////////////////////////////////////////////////////////////
cub::Status FooStateFactory::getFailCauseByEvent(const ev::Event&) const
{
    return TSL_SUCCESS;
}

///////////////////////////////////////////////////////////////////
cub::Status FooStateFactory::getInterruptCauseByEvent(const ev::Event&) const
{
    return TSL_SUCCESS;
}

///////////////////////////////////////////////////////////////////
cub::Status FooStateFactory::getPreemptCauseByEvent(const ev::Event&) const
{
    return TSL_SUCCESS;
}

///////////////////////////////////////////////////////////////////
bool FooStateFactory::isTransEvent(const ev::EventId) const
{
    return false;
}

///////////////////////////////////////////////////////////////////
bool FooStateFactory::isStrategyEvent(const ev::EventId eventId) const
{
    switch(eventId)
    {
    case EV_EVENT1: return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////
bool FooStateFactory::isTerminalEvent(const ev::EventId) const
{
    return false;

}

///////////////////////////////////////////////////////////////////
void FooStateFactory::destroyState(State*)
{
}

///////////////////////////////////////////////////////////////////
void FooStateFactory::reset()
{
}

L4_NS_END
