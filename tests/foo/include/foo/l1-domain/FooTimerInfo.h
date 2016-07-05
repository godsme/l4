#ifndef H774E8C7D_4B31_4BBB_B5E4_0E1BB2E2338F
#define H774E8C7D_4B31_4BBB_B5E4_0E1BB2E2338F

#include <cub/gof/Singleton.h>
#include <state/l4.h>
#include <trans-dsl/sched/concept/TimerInfo.h>

L4_NS_BEGIN

struct FooTimerInfo : tsl::TimerInfo, cub::Singleton<FooTimerInfo>
{
    OVERRIDE(cub::U32 getTimerLen(const tsl::TimerId) const);
};

L4_NS_END

#endif /* H774E8C7D_4B31_4BBB_B5E4_0E1BB2E2338F */
