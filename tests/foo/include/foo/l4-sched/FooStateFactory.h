#ifndef H7281C760_7346_4E54_ADA4_BF0C6CA4AC11
#define H7281C760_7346_4E54_ADA4_BF0C6CA4AC11

#include <event/concept/EventId.h>
#include <state/l4.h>
#include <trans-dsl/ext/mutex/impl/AbstractTransMutexScheduler.h>

FWD_DECL_TSL(TransMutexAvailNotifier);
FWD_DECL_TSL(TransSignalScheduler);

L4_NS_BEGIN

struct FooStateFactory
    : StateFactory
    , private tsl::AbstractTransMutexScheduler
{
   FooStateFactory();
   ~FooStateFactory();

   State* createInitialState();
   State* createFailState(const cub::Status, const ev::Event&);
   State* createStableState(const StateId);
   State* createUnstableState(const ev::EventId);
   State* createPriUnstableState(const ev::EventId);

   cub::Status destroyState(State*);
   void reset();

   TransStrategyDecisionMaker* getStrategyMaker(const ev::EventId);
   FailedRequestListener* getFailedRequestListener(const ev::EventId);

   cub::Status getFailCauseByEvent(const ev::Event&) const;
   cub::Status getInterruptCauseByEvent(const ev::Event&) const;
   cub::Status getPreemptCauseByEvent(const ev::Event&) const;

   bool isTransEvent(const ev::EventId);
   bool isStrategyEvent(const ev::EventId eventId);
   bool isRunningState(const StateId& ) const;
   bool isTerminalEvent(const ev::EventId) const;

private:
   State* doCreateState(const StateId);
   UnstableState* doCreateUnstableState(const ev::EventId);
   void updateState(UnstableState* newState);

   void destroy();
   void destroyPriState();
   void updateFailStateId();

   State* prepFailTrans(UnstableState* failState, const cub::Status failCause);
   State* resumeInitState();

private:
   State* state;
   UnstableState* priState;

   StateId stateId;

   enum { SIZE_OF_STATE_SPACE     = 8
        , SIZE_OF_LISTENER_SPACE  = 8 };

   cub::U8 u[SIZE_OF_STATE_SPACE];
   cub::U8 listener[SIZE_OF_LISTENER_SPACE];

protected:
   typedef tsl::TransMutexAvailNotifier TransMutexAvailNotifier;
   typedef tsl::TransSignalScheduler TransSignalScheduler;

private:
   USE_ROLE(TransMutexAvailNotifier);
   USE_ROLE(TransSignalScheduler);
};

L4_NS_END

#endif /* H7281C760_7346_4E54_ADA4_BF0C6CA4AC11 */
