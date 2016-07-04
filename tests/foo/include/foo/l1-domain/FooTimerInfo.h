#ifndef H774E8C7D_4B31_4BBB_B5E4_0E1BB2E2338F
#define H774E8C7D_4B31_4BBB_B5E4_0E1BB2E2338F

#include <state/l4.h>
#include <trans-dsl/sched/concept/TimerInfo.h>

L4_NS_BEGIN

struct FooTimerInfo : tsl::TimerInfo
{
    OVERRIDE(cub::U32 getTimerLen(const tsl::TimerId) const);
};

L4_NS_END

#endif /* H774E8C7D_4B31_4BBB_B5E4_0E1BB2E2338F */
