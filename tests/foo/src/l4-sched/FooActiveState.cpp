#include <foo/l4-sched/FooActiveState.h>
#include <foo/l4-sched/FooEvent.h>
#include <state/TransStrategyDef.h>

L4_NS_BEGIN

using namespace cub;

///////////////////////////////////////////////////////////
BEGIN_STATE_STRATEGY_TABLE(FooActiveState)
   STABLE_ACCEPTED_EVENT(EV_EVENT1)
END_STATE_STRATEGY_TABLE()


L4_NS_END
