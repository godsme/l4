#ifndef HE3CE1E8F_CC6B_4F3B_A14D_08F7A54C4360
#define HE3CE1E8F_CC6B_4F3B_A14D_08F7A54C4360

#include <state/State.h>

L4_NS_BEGIN

struct StableState : State
{
   OVERRIDE(bool   isStable() const);
   OVERRIDE(cub::Status enter(StateMachine&));
   OVERRIDE(cub::Status handleEvent(StateMachine&, const ev::Event&));
   OVERRIDE(cub::Status leave(StateMachine&, const cub::Status));
   OVERRIDE(void   kill(StateMachine&, const cub::Status));
};

L4_NS_END

#endif /* HE3CE1E8F_CC6B_4F3B_A14D_08F7A54C4360 */
