#ifndef HF43DEE74_1DEC_4BB6_95E9_CEDBE4BC67EA
#define HF43DEE74_1DEC_4BB6_95E9_CEDBE4BC67EA

#include <state/l4.h>

#include <cub/dci/Role.h>

#include <event/event.h>
#include <state/StateId.h>
#include <state/TransStrategy.h>
#include <state/TransStrategyDecisionMaker.h>
#include <trans-dsl/TslStatus.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

struct StateMachine;

struct StateMachine;

struct State : TransStrategyDecisionMaker
{
   ABSTRACT(StateId getId() const);
   ABSTRACT(bool   isStable() const);

   ABSTRACT(cub::Status enter(StateMachine&));
   ABSTRACT(cub::Status handleEvent(StateMachine&, const ev::Event&));
   ABSTRACT(cub::Status leave(StateMachine&, const cub::Status cause = TSL_SUCCESS));
   ABSTRACT(void   kill(StateMachine&, const cub::Status cause = TSL_SUCCESS));

   virtual cub::Status onIdle(StateMachine&) { return TSL_SUCCESS; }
   virtual cub::Status onIdleTransSchedule(StateMachine&) { return TSL_SUCCESS; }
   virtual void onReentry(StateMachine&) {}
   virtual bool isInitState() const { return false; }

   virtual bool ignoreFailure() const { return false; }
};

L4_NS_END

#endif /* HF43DEE74_1DEC_4BB6_95E9_CEDBE4BC67EA */
