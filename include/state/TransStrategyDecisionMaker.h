#ifndef H5187FA6C_B930_4786_AF35_461340963B98
#define H5187FA6C_B930_4786_AF35_461340963B98

#include <cub/dci/Role.h>
#include <event/event.h>
#include <state/TransStrategy.h>
#include <state/l4.h>
#include <trans-dsl/sched/concept/InstanceId.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

DEFINE_ROLE(TransStrategyDecisionMaker)
{
   ABSTRACT(TransStrategy getStrategy(const tsl::InstanceId, const ev::Event&) const);
};

L4_NS_END

#endif /* H5187FA6C_B930_4786_AF35_461340963B98 */
