#include <cub/log/log.h>
#include <foo/l2-context/FooAsynAction2.h>
#include <event/concept/Event.h>
#include <trans-dsl/TslStatus.h>
#include <foo/l4-sched/FooEvent.h>
#include <trans-dsl/sched/concept/TransactionInfo.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction2::exec(const tsl::TransactionInfo& info)
{
    DBG_LOG("start Foo Instance = %p", info.getInstanceId());

    WAIT_ON(EV_EVENT2, handleEvent2);

    return TSL_CONTINUE;
}

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction2::handleEvent2(const tsl::TransactionInfo& info, const ev::Event& event)
{
    DBG_LOG("handle event: Foo Instance = %p", info.getInstanceId());

    return TSL_SUCCESS;
}

L4_NS_END
