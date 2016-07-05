#include <foo/l4-sched/FooEvent.h>
#include <foo/l4-sched/state/unstable/FooTrans1State.h>
#include <foo/l4-sched/FooStateId.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////////////////////////////////////
__BEGIN_STATIC_STRATEGY(FooTrans1StaticStrategy)
__STATE_STRATEGY(EV_EVENT1_T, TS_DISCARD)
__STATE_STRATEGY(EV_EVENT4_T, TS_BUFFER)
__STATE_STRATEGY(EV_EVENT_R,  TS_PREEMPT)
__END_STATIC_STRATEGY()

///////////////////////////////////////////////////////////////////////////////////////////////////
TransStrategy FooTrans1RuntimeStrategy::getStrategy(tsl::InstanceId, com::Unknown*, const ev::Event&)
{
    return TS_UNKNOWN;
}


L4_NS_END
