#ifndef H5187FA6C_B930_4786_AF35_461340963B98
#define H5187FA6C_B930_4786_AF35_461340963B98

#include <cub/dci/Role.h>
#include <event/concept/Event.h>
#include <event/event.h>
#include <state/TransStrategy.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

DEFINE_ROLE(TransStrategyDecisionMaker)
{
   ABSTRACT(TransStrategy getStrategy(const ev::Event&) const);
};

///////////////////////////////////////////////////////////////////
#define __BEGIN_STATIC_STRATEGY(name)                   \
TransStrategy name::getStrategy(const ev::Event& event) \
{ switch(event.getEventId()) {

///////////////////////////////////////////////////////////////////
#define __STATE_STRATEGY(eventId, strategy) \
case eventId : return strategy;

///////////////////////////////////////////////////////////////////
#define __END_STATIC_STRATEGY() \
 default: break; }              \
 return TS_UNKNOWN; }

///////////////////////////////////////////////////////////////////

L4_NS_END

#endif /* H5187FA6C_B930_4786_AF35_461340963B98 */
