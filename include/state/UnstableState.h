#ifndef H76437B8C_2657_47D8_81E1_63F75588B31A
#define H76437B8C_2657_47D8_81E1_63F75588B31A

#include <state/l4.h>
#include <cub/base/Keywords.h>

L4_NS_BEGIN

struct UnstableState : State
{
   UnstableState();

   void startFrom(const StateId from);

   //////////////////////////////////////////////////////////////////////////
   DEFAULT(void, updateMutexScheduler(tsl::TransMutexScheduler&));
   DEFAULT(void, updateSignalScheduler(tsl::TransSignalScheduler&));

   DEFAULT(void, init(const tsl::InstanceId&));
   DEFAULT(void, destroy());

private:
   OVERRIDE(bool isStable() const);

   OVERRIDE(cub::Status enter(StateMachine&));
   OVERRIDE(cub::Status handleEvent(StateMachine&, const ev::Event&));
   OVERRIDE(cub::Status leave(StateMachine&, const cub::Status cause));
   OVERRIDE(void kill(StateMachine& context, const cub::Status cause));

private:
   cub::Status finalProcess(StateMachine&, const tsl::ActionStatus& status);

private:
   StateId from;

private:
    ABSTRACT(cub::Status doEnter());
    ABSTRACT(cub::Status doHandleEvent(const ev::Event&));
    ABSTRACT(cub::Status doLeave(const cub::Status));
    ABSTRACT(void doKill(const cub::Status));
    ABSTRACT(StateId getToState() const);
};

//////////////////////////////////////////////////////////////////////////

#define DECL_UNSTABLE_STATE_STRATEGY()                             \
    static TransStrategyDecisionMaker* getStrategyDecisionMaker(); \
    OVERRIDE(TransStrategy                                         \
    getStrategy(const InstanceId, const Event&) const)

//////////////////////////////////////////////////////////////////////////

#define BEGIN_UNSTABLE_STRATEGY_TABLE(state)                       \
    namespace                                                      \
    {                                                              \
        struct state##Strategy : TransStrategyDecisionMaker        \
        {                                                          \
            OVERRIDE(TransStrategy getStrategy                     \
                (const InstanceId, const Event&) const);           \
        };                                                         \
                                                                   \
        BEGIN_STATE_STRATEGY_TABLE(state##Strategy)


#define END_UNSTABLE_STRATEGY_TABLE(state)                         \
    END_STATE_STRATEGY_TABLE()                                     \
}                                                                  \
                                                                   \
    TransStrategyDecisionMaker* state::getStrategyDecisionMaker()  \
    {                                                              \
        static state##Strategy instance;                           \
        return &instance;                                          \
    }
L4_NS_END

#endif /* H76437B8C_2657_47D8_81E1_63F75588B31A */
