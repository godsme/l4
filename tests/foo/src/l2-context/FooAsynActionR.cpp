#include <foo/l2-context/FooAsynActionR.h>
#include <event/concept/Event.h>
#include <trans-dsl/TslStatus.h>
#include <foo/l4-sched/FooEvent.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
cub::Status FooAsynActionR::exec(const tsl::TransactionInfo&)
{
    WAIT_ON(EV_EVENT_R, handleEventR);

    return TSL_CONTINUE;
}

///////////////////////////////////////////////////////////////////
cub::Status FooAsynActionR::handleEventR(const tsl::TransactionInfo&, const ev::Event& event)
{
    return TSL_SUCCESS;
}

L4_NS_END
