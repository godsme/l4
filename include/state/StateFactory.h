#ifndef HFDE00758_D12B_4D76_A258_CA0CBB960F8A
#define HFDE00758_D12B_4D76_A258_CA0CBB960F8A

#include <cub/base/Status.h>
#include <cub/dci/Role.h>
#include <event/concept/EventId.h>
#include <state/l4.h>
#include <state/StateId.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

struct State;

struct State;
struct PostProcessHandler;
struct FailedRequestListener;
struct TransStrategyDecisionMaker;


DEFINE_ROLE(StateFactory)
{

   ABSTRACT(State* createInitialState());
   ABSTRACT(State* createStableState(const StateId));
   ABSTRACT(State* createFailState(const cub::Status, const ev::Event&));
   ABSTRACT(State* createUnstableState(const ev::EventId));
   ABSTRACT(State* createPriUnstableState(const ev::EventId));

//   ABSTRACT(bool isTransEvent(const ev::EventId) const);
//   ABSTRACT(bool isStrategyEvent(const ev::EventId) const);

   ABSTRACT(const TransStrategyDecisionMaker* getStrategyMaker(const ev::EventId));
   ABSTRACT(FailedRequestListener* getFailedRequestListener(const ev::EventId));

   ABSTRACT(cub::Status getFailCauseByEvent(const ev::Event&) const);
   ABSTRACT(cub::Status getInterruptCauseByEvent(const ev::Event&) const);
   ABSTRACT(cub::Status getPreemptCauseByEvent(const ev::Event&) const);

   ABSTRACT(void destroyState(State*));

   ABSTRACT(void reset());
};

L4_NS_END

#endif /* HFDE00758_D12B_4D76_A258_CA0CBB960F8A */
