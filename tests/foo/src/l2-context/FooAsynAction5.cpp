#include <foo/l2-context/FooAsynAction5.h>
#include <event/concept/Event.h>
#include <trans-dsl/TslStatus.h>
#include <foo/l4-sched/FooEvent.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction5::exec(const tsl::TransactionInfo&)
{
    WAIT_ON(EV_EVENT5, handleEvent5);

    return TSL_CONTINUE;
}

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction5::handleEvent5(const tsl::TransactionInfo&, const ev::Event& event)
{
    return TSL_SUCCESS;
}

L4_NS_END
