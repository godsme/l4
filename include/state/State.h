#ifndef HF43DEE74_1DEC_4BB6_95E9_CEDBE4BC67EA
#define HF43DEE74_1DEC_4BB6_95E9_CEDBE4BC67EA

#include <state/l4.h>

#include <cub/dci/Role.h>

#include <event/event.h>
#include <state/StateId.h>
#include <state/TransStrategy.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <trans-dsl/TslStatus.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

struct StateMachine;

DEFINE_ROLE(State)
{
   ABSTRACT(StateId getId() const);
   ABSTRACT(bool    isStable() const);

   ABSTRACT(cub::Status enter(StateMachine&));
   ABSTRACT(cub::Status handleEvent(StateMachine&, const ev::Event&));
   ABSTRACT(cub::Status leave(StateMachine&, const cub::Status cause = TSL_SUCCESS));
   ABSTRACT(void   kill(StateMachine&, const cub::Status cause = TSL_SUCCESS));

   ABSTRACT(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const);

   DEFAULT(cub::Status, onIdle(StateMachine&));
   DEFAULT(cub::Status, onIdleTransSchedule(StateMachine&));
   DEFAULT(void, onReentry(StateMachine&));

   DEFAULT(bool, isInitState() const);
   DEFAULT(bool, isFinalState() const);

   DEFAULT(bool, ignoreFailure() const);
};

L4_NS_END

#endif /* HF43DEE74_1DEC_4BB6_95E9_CEDBE4BC67EA */
