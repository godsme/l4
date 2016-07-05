#include <foo/l2-context/FooAsynAction1.h>
#include <cub/base/Assertions.h>
#include <event/concept/Event.h>
#include <trans-dsl/TslStatus.h>
#include <foo/l4-sched/FooEvent.h>
#include <cub/log/log.h>
#include <foo/l3-trans/common/Context0.h>
#include <foo/l3-trans/trans1/Context1.h>
#include <foo/l3-trans/trans1/Context2.h>
#include <foo/l3-trans/trans2/Context3.h>
#include <trans-dsl/sched/concept/TransactionInfo.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction1::exec(const tsl::TransactionInfo& info)
{
    auto context0 = info.toRole<Context0>();
    CUB_ASSERT_VALID_PTR(context0);

    DBG_LOG("Context 0 --------------------- = %d", context0->getValue());

    auto context1 = info.toRole<Context1>();
    CUB_ASSERT_VALID_PTR(context1);

    DBG_LOG("Context 1 --------------------- = %d", context1->getValue());

    auto context2 = info.toRole<Context2>();
    CUB_ASSERT_VALID_PTR(context1);

    DBG_LOG("Context 2 --------------------- = %d", context2->getValue());

    auto context3 = info.toRole<Context3>();
    CUB_ASSERT_TRUE(context3 == nullptr);


    WAIT_ON(EV_EVENT1_T, handleEvent1);

    return TSL_CONTINUE;
}

///////////////////////////////////////////////////////////////////
cub::Status FooAsynAction1::handleEvent1(const tsl::TransactionInfo&, const ev::Event& event)
{
    const Event1* msg = (const Event1*) event.getMsg();

    return TSL_SUCCESS;
}

L4_NS_END
