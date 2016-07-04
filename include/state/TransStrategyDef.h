#ifndef H6327431F_6BC7_4C77_8A18_3B9F30C6C90B
#define H6327431F_6BC7_4C77_8A18_3B9F30C6C90B

#include <state/l4.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <event/concept/Event.h>

L4_NS_BEGIN

inline void dummyInstanceId(const tsl::InstanceId) {}

#define BEGIN_STATE_STRATEGY_TABLE(state)            \
TransStrategy state::getStrategy                     \
    (const tsl::InstanceId iid, const ev::Event& event) const \
{                                                    \
    dummyInstanceId(iid);                            \
    switch(event.getEventId()) {

#define END_STATE_STRATEGY_TABLE()                   \
    }                                                \
    return TS_NIL;                                   \
}

#define STATE_STRATEGY(eventId, strategy)            \
   case eventId: return strategy;

#define DEFAULT_STRATEGY(strategy)                   \
   case 0:                                           \
   default : return strategy;

#define DEFAULT_STRATEGY_MAKER(strategyMaker)       \
default: return strategyMaker::getInstance().getStrategy(iid, event);

#define STABLE_ACCEPTED_EVENT(eventId) STATE_STRATEGY(eventId, TS_REPLACE)

L4_NS_END

#endif /* H6327431F_6BC7_4C77_8A18_3B9F30C6C90B */
