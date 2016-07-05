#include <foo/l4-sched/state/stable/FooActiveState.h>
#include <foo/l4-sched/FooEvent.h>
#include <state/TransStrategyDef.h>

L4_NS_BEGIN

using namespace cub;

///////////////////////////////////////////////////////////
BEGIN_STATE_STRATEGY_TABLE(FooActiveState)
   STABLE_ACCEPTED_EVENT(EV_EVENT4_T)
   STABLE_ACCEPTED_EVENT(EV_EVENT_R)
END_STATE_STRATEGY_TABLE()

L4_NS_END
