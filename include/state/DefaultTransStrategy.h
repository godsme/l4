#ifndef H5D99EF39_F494_4001_A895_35EA8F7ACB28
#define H5D99EF39_F494_4001_A895_35EA8F7ACB28

#include <state/TransStrategy.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <event/event.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

#define __DEF_TS_STATIC_STRATEGY(prefix, strategy) \
struct prefix ## StaticStrategy                    \
{                                                  \
    static TransStrategy getStrategy               \
         (const ev::Event&)                        \
    {                                              \
        return TS_##strategy;                      \
    }                                              \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
__DEF_TS_STATIC_STRATEGY(RejectAll, REJECT);


#define __DEF_TS_RUNTIME_STRATEGY(prefix, strategy) \
struct prefix ## RuntimeStrategy                    \
{                                                   \
    static TransStrategy getStrategy                \
         ( tsl::InstanceId                          \
         , com::Unknown*                            \
         , const ev::Event&)                        \
    {                                               \
        return TS_##strategy;                       \
    }                                               \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
__DEF_TS_RUNTIME_STRATEGY(RejectAll, REJECT);

L4_NS_END

#endif /* H5D99EF39_F494_4001_A895_35EA8F7ACB28 */
