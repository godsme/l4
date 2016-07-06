#ifndef HACFEE222_5875_4F15_A137_89B6E908B869
#define HACFEE222_5875_4F15_A137_89B6E908B869

#include <state/StateId.h>

L4_NS_BEGIN

enum : StateId
{
    STATE_Trans1,
    STATE_Trans2,
    STATE_Release,

    STATE_Idle,
    STATE_Active,
    STATE_Final,

    INVALID_STATE_TYPE,  // add state type before STATE_TYPE_NUM
    STATE_UNSTABLE = 0xFE,
};

L4_NS_END

#endif /* HACFEE222_5875_4F15_A137_89B6E908B869 */
