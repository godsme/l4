#ifndef H5A78E912_2FE1_4D4A_899A_82B0C1D325E8
#define H5A78E912_2FE1_4D4A_899A_82B0C1D325E8

#include <state/UnstableState.h>
#include <trans-dsl/tsl.h>

FWD_DECL_TSL(Transaction);

L4_NS_BEGIN

struct TransMutexScheduler;
struct TransSignalScheduler;

struct UnstableTransState : UnstableState
{
    explicit UnstableTransState(const StateId to)
      : UnstableState(to)
    {}

    OVERRIDE(cub::Status doEnter());
    OVERRIDE(cub::Status doLeave(const cub::Status cause));
    OVERRIDE(cub::Status doHandleEvent(const ev::Event&));
    OVERRIDE(void doKill(const cub::Status cause));

private:
    cub::Status postprocess(const ev::Event& event);

    virtual cub::Status postTransaction(const ev::Event&);

protected:
    typedef tsl::Transaction Transaction;

    USE_ROLE(Transaction);

protected:
    virtual TransStrategy getRuntimeStrategy(const tsl::InstanceId, const ev::Event&) const;
};

//////////////////////////////////////////////////////////////////////////

#define DECL_UNSTABLE_TRANS_METHODS()                             \
    DECL_UNSTABLE_STATE_STRATEGY()

//////////////////////////////////////////////////////////////////////////

#define BEGIN_UNSTABLE_TRANS_STATE_STRATEGY_TABLE(transState)       \
    BEGIN_UNSTABLE_STRATEGY_TABLE(transState)

#define END_UNSTABLE_TRANS_STATE_STRATEGY_TABLE(transState)         \
    END_UNSTABLE_STRATEGY_TABLE(transState)                         \
    TransStrategy transState::getStrategy                           \
        (const InstanceId iid, const Event& event) const            \
    {                                                               \
        TransStrategy strategy = getRuntimeStrategy(iid, event);    \
        if(strategy != TS_UNKNOWN) return strategy;                 \
        return getStrategyDecisionMaker()->getStrategy(iid, event); \
    }

L4_NS_END

#endif /* H5A78E912_2FE1_4D4A_899A_82B0C1D325E8 */
