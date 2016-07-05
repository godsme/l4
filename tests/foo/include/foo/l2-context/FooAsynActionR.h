#ifndef HBA846818_B80E_4033_B3C6_9FB1D087E84F
#define HBA846818_B80E_4033_B3C6_9FB1D087E84F

#include <state/l4.h>
#include <trans-dsl/action/SimpleAsyncActionHelper.h>

L4_NS_BEGIN

DEF_SIMPLE_ASYNC_ACTION(FooAsynActionR)
{
    OVERRIDE(cub::Status exec(const tsl::TransactionInfo&));

private:
    cub::Status handleEventR(const tsl::TransactionInfo&, const ev::Event&);
};

L4_NS_END

#endif /* HBA846818_B80E_4033_B3C6_9FB1D087E84F */
