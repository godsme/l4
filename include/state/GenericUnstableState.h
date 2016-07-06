#ifndef H13C1DD8D_CEAB_4BC8_BC14_A0D14CD77D91
#define H13C1DD8D_CEAB_4BC8_BC14_A0D14CD77D91

#include <cub/gof/Singleton.h>
#include <state/l4.h>
#include <state/UnstableTransState.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <event/event.h>
#include <state/TransStrategyDecisionMaker.h>
#include <state/UnstableStateInfo.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
template < StateId P_SID
         , StateId P_TO_STATE
         , typename P_TRANS
         , typename P_STATIC_STRATEGY_TABLE
         , typename P_RUNTIME_STRATEGY_TABLE>
struct GenericUnstableState : UnstableTransState
{
private:
    template <typename STRATEGY_TABLE>
    struct TransStaticStrategy
            : TransStrategyDecisionMaker
            , cub::Singleton< TransStaticStrategy<STRATEGY_TABLE> >
    {
        OVERRIDE(TransStrategy getStrategy(const ev::Event& event) const)
        {
            return STRATEGY_TABLE::getStrategy(event);
        }
    };

    typedef TransStaticStrategy<P_STATIC_STRATEGY_TABLE> StaticStrategy;

public:
    GenericUnstableState(tsl::InstanceId iid)
        : UnstableTransState(P_TO_STATE)
        , trans(iid)
    {
    }

    OVERRIDE(StateId getId() const)
    {
        return P_SID;
    }

    static StateId getStateId()
    {
        return P_SID;
    }

    static const TransStrategyDecisionMaker* getStrategyDecisionMaker()
    {
        return &StaticStrategy::getInstance();
    }

    OVERRIDE(TransStrategy getStrategy(const tsl::InstanceId iid, const ev::Event& event) const)
    {
        TransStrategy strategy = P_RUNTIME_STRATEGY_TABLE::getStrategy(iid, trans.getUserContext(), event);
        if(strategy != TS_UNKNOWN)
        {
            return strategy;
        }

        return P_STATIC_STRATEGY_TABLE::getStrategy(event);
    }

protected:
    P_TRANS trans;

private:
    IMPL_ROLE_WITH_OBJ(Transaction, trans);
};

L4_NS_END

#endif /* H13C1DD8D_CEAB_4BC8_BC14_A0D14CD77D91 */
