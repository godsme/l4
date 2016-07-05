#ifndef H5187FA6C_B930_4786_AF35_461340963B98
#define H5187FA6C_B930_4786_AF35_461340963B98

#include <cub/dci/Role.h>
#include <event/event.h>
#include <state/TransStrategy.h>
#include <state/l4.h>
#include <trans-dsl/sched/concept/InstanceId.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

DEFINE_ROLE(TransStrategyDecisionMaker)
{
   ABSTRACT(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const);
};

///////////////////////////////////////////////////////////////////
#define STATE_STRATEGY_DECL() \
    OVERRIDE(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const)

#define STATE_RUNTIME_STRATEGY_DECL() \
    OVERRIDE(TransStrategy getSpecialRuntimeStrategy(const tsl::InstanceId, const ev::Event&) const)

//////////////////////////////////////////////////////////////////////////
#define STATIC_STRATEGY_DECL(p_name)                  \
struct p_name : TransStrategyDecisionMaker            \
{                                                     \
    static TransStrategyDecisionMaker& getInstance(); \
    STATE_STRATEGY_DECL();                            \
}

//////////////////////////////////////////////////////////////////////////
#define __STRATEGY_TABLE() switch(event.getEventId())

//////////////////////////////////////////////////////////////////////////
#define __BEGIN_STRATEGY_TABLE() __STRATEGY_TABLE() {
#define __END_STRATEGY_TABLE()  default: return TS_UNKNOWN; }

//////////////////////////////////////////////////////////////////////////
#define __DEF_STATIC_STRATEGY(p_name)                \
TransStrategyDecisionMaker& p_name::getInstance()    \
{                                                    \
    static p_name instance;                          \
    return instance;                                 \
}                                                    \
TransStrategy p_name::getStrategy(const tsl::InstanceId iid, const ev::Event& event) const

//////////////////////////////////////////////////////////////////////////
#define BEGIN_STATIC_STRATEGY_DEF(p_name)           \
__DEF_STATIC_STRATEGY(p_name)  {                     \
    __BEGIN_STRATEGY_TABLE()

//////////////////////////////////////////////////////////////////////////
#define END_STATIC_STRATEGY_DEF()                    \
default:break;}                                      \
    return TS_UNKNOWN; }

#define END_STATIC_STRATEGY_DEF_WITHOUT_DEFAULT()    \
    }                                                \
}

L4_NS_END

#endif /* H5187FA6C_B930_4786_AF35_461340963B98 */
