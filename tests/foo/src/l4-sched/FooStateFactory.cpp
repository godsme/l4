#include <foo/l4-sched/FooStateFactory.h>
#include <foo/l4-sched/state/stable/FooIdleState.h>
#include <foo/l4-sched/state/stable/FooActiveState.h>
#include <foo/l4-sched/FooEvent.h>
#include <foo/l4-sched/FooTransObjectFactory.h>
#include <state/StateId.h>
#include <state/UnstableState.h>
#include <trans-dsl/TslStatus.h>
#include <cub/mem/Placement.h>
#include <cub/log/log.h>
#include <foo/l4-sched/rejecter/Event1FailedRequestListener.h>
#include <foo/l4-sched/state/unstable/FooReleaseState.h>
#include <foo/l4-sched/state/unstable/FooTrans1State.h>
#include <foo/l4-sched/state/unstable/FooTrans2State.h>

L4_NS_BEGIN

using namespace cub;

namespace
{
    union StableStateSpace
    {
        cub::Placement<FooIdleState> idle;
        cub::Placement<FooActiveState> active;
    };
}

///////////////////////////////////////////////////////////////////
FooStateFactory::FooStateFactory(tsl::InstanceId iid) : iid(iid)
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
State* FooStateFactory::createFailState(const cub::Status status, const ev::Event&)
{
    DBG_LOG("create fail state: %x", status);
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
        return new (&u) FooActiveState;
    default:
        break;
    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////
StateId FooStateFactory::getStateIdByEvent(const ev::EventId eid) const
{
    switch(eid)
    {
    case EV_EVENT1_T: return STATE_Trans1;
    case EV_EVENT4_T: return STATE_Trans2;
    case EV_EVENT_R : return STATE_Release;
    }

    return INVALID_STATE_TYPE;
}

///////////////////////////////////////////////////////////////////
State* FooStateFactory::createUnstableState(const ev::EventId eid)
{
    return FooTransObjectFactory::createState(iid, getStateIdByEvent(eid));
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

    switch(eid)
    {
    case EV_EVENT1_T: return FooTrans1State::getStrategyDecisionMaker();
    case EV_EVENT4_T: return FooTrans2State::getStrategyDecisionMaker();
    case EV_EVENT_R : return FooReleaseState::getStrategyDecisionMaker();
    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////
FailedRequestListener* FooStateFactory::getFailedRequestListener(const ev::EventId eventId)
{
    switch(eventId)
    {
    case EV_EVENT1_T: return &(Event1Rejector::getInstance());
    }

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
void FooStateFactory::destroyState(State* state)
{
    if(!state->isStable())
    {
        delete state;
    }
}

///////////////////////////////////////////////////////////////////
void FooStateFactory::reset()
{
}

L4_NS_END
