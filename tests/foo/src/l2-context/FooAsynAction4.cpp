#include <foo/l2-context/FooAsynAction4.h>
#include <event/concept/Event.h>
#include <trans-dsl/TslStatus.h>
#include <foo/l4-sched/FooEvent.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction4::exec(const tsl::TransactionInfo&)
{
    WAIT_ON(EV_EVENT4_T, handleEvent4);

    return TSL_CONTINUE;
}

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction4::handleEvent4(const tsl::TransactionInfo&, const ev::Event& event)
{
    return TSL_SUCCESS;
}

L4_NS_END
