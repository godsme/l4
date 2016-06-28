#ifndef H77D633F3_D232_4E47_8266_BFB8C9223C0A
#define H77D633F3_D232_4E47_8266_BFB8C9223C0A

#include <state/l4.h>
#include <cub/dci/Role.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <event/event.h>
#include <state/StateId.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

DEFINE_ROLE(FailedRequestListener)
{
   DEFAULT(void, onReject(const tsl::InstanceId iid, const ev::Event& event, const StateId sid) const);
   DEFAULT(void, onDiscard(const tsl::InstanceId iid, const ev::Event& event, const StateId sid) const);
};

L4_NS_END

#endif /* H77D633F3_D232_4E47_8266_BFB8C9223C0A */
