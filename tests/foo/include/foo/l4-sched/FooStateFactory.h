#ifndef H7281C760_7346_4E54_ADA4_BF0C6CA4AC11
#define H7281C760_7346_4E54_ADA4_BF0C6CA4AC11

#include <cub/base/Keywords.h>
#include <event/concept/EventId.h>
#include <state/StateFactory.h>
#include <trans-dsl/ext/mutex/impl/AbstractTransMutexScheduler.h>

FWD_DECL_TSL(TransMutexAvailNotifier);
FWD_DECL_TSL(TransSignalScheduler);

L4_NS_BEGIN

struct UnstableState;

struct FooStateFactory
    : StateFactory
    , private tsl::AbstractTransMutexScheduler
{
   FooStateFactory();
   ~FooStateFactory();

   OVERRIDE(State* createInitialState());
   OVERRIDE(State* createFailState(const cub::Status, const ev::Event&));
   OVERRIDE(State* createStableState(const StateId));
   OVERRIDE(State* createUnstableState(const ev::EventId));
   OVERRIDE(State* createPriUnstableState(const ev::EventId));

   OVERRIDE(TransStrategyDecisionMaker* getStrategyMaker(const ev::EventId));
   OVERRIDE(FailedRequestListener* getFailedRequestListener(const ev::EventId));

   OVERRIDE(cub::Status getFailCauseByEvent(const ev::Event&) const);
   OVERRIDE(cub::Status getInterruptCauseByEvent(const ev::Event&) const);
   OVERRIDE(cub::Status getPreemptCauseByEvent(const ev::Event&) const);

   OVERRIDE(bool isTransEvent(const ev::EventId) const);
   OVERRIDE(bool isStrategyEvent(const ev::EventId eventId) const);
   OVERRIDE(bool isTerminalEvent(const ev::EventId) const);

   OVERRIDE(void destroyState(State*));
   OVERRIDE(void reset());

private:
//   State* doCreateState(const StateId);
//   UnstableState* doCreateUnstableState(const ev::EventId);
//   void updateState(UnstableState* newState);
//
//   void destroy();
//   void destroyPriState();
//   void updateFailStateId();
//
//   State* prepFailTrans(UnstableState* failState, const cub::Status failCause);
//   State* resumeInitState();

private:
   State* state;

   StateId stateId;

   enum { SIZE_OF_STATE_SPACE     = 8
        , SIZE_OF_LISTENER_SPACE  = 8 };

   cub::U8 u[SIZE_OF_STATE_SPACE];
   cub::U8 listener[SIZE_OF_LISTENER_SPACE];

protected:
   typedef tsl::TransMutexAvailNotifier TransMutexAvailNotifier;
   typedef tsl::TransSignalScheduler TransSignalScheduler;

private:
   USE_ROLE(TransSignalScheduler);
};

L4_NS_END

#endif /* H7281C760_7346_4E54_ADA4_BF0C6CA4AC11 */
