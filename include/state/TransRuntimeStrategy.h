#ifndef H35947E6E_467F_4B63_B1CA_A7CB0F645F63
#define H35947E6E_467F_4B63_B1CA_A7CB0F645F63

#include <cub/dci/Role.h>
#include <cub/dci/Unknown.h>
#include <state/l4.h>
#include <state/TransStrategy.h>
#include <event/event.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

DEFINE_ROLE(TransRuntimeStrategy)
{
    ABSTRACT(TransStrategy getStrategy(tsl::InstanceId, com::Unknown*, const ev::Event&));
};

L4_NS_END

#endif /* H35947E6E_467F_4B63_B1CA_A7CB0F645F63 */
