#ifndef HA41145A5_A776_4536_924A_E4DFA16897EF
#define HA41145A5_A776_4536_924A_E4DFA16897EF

#include <foo/l4-sched/FooStateFactory.h>
#include <state/EventQueue.h>
#include <state/TransStateMachine.h>

L4_NS_BEGIN

struct FooStateMachine
        : private FooStateFactory
        , private EventQueue
        , TransStateMachine
{
    explicit FooStateMachine(tsl::InstanceId);

private:
    typedef tsl::TransMutexAvailNotifier TransMutexAvailNotifier;
    typedef tsl::TransSignalScheduler TransSignalScheduler;

    IMPL_ROLE(TransMutexAvailNotifier);
    IMPL_ROLE(TransSignalScheduler);
};

L4_NS_END

#endif /* HA41145A5_A776_4536_924A_E4DFA16897EF */
