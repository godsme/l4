#ifndef HBDF6F0BC_EE59_448E_8C92_8D6F964943CB
#define HBDF6F0BC_EE59_448E_8C92_8D6F964943CB

#include <state/l4.h>
#include <trans-dsl/action/SimpleAsyncActionHelper.h>

L4_NS_BEGIN

DEF_SIMPLE_ASYNC_ACTION(FooAsynAction6)
{
    OVERRIDE(cub::Status exec(const tsl::TransactionInfo&));

private:
    cub::Status handleEvent6(const tsl::TransactionInfo&, const ev::Event&);
};

L4_NS_END

#endif /* HBDF6F0BC_EE59_448E_8C92_8D6F964943CB */
