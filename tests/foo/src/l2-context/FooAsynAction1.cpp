#include <foo/l2-context/FooAsynAction1.h>
#include <event/concept/Event.h>
#include <trans-dsl/TslStatus.h>
#include <foo/l4-sched/FooEvent.h>

#include <iostream>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction1::exec(const tsl::TransactionInfo&)
{
    WAIT_ON(EV_EVENT1, handleEvent1);
    return TSL_CONTINUE;
}

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction1::handleEvent1(const tsl::TransactionInfo&, const ev::Event& event)
{
    const Event1* msg = (const Event1*) event.getMsg();

    std::cout << msg->a <<", " << msg->b << std::endl;

    return TSL_SUCCESS;
}

L4_NS_END
