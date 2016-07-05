#ifndef HF7A8BCE1_C419_424E_B625_A47D5C30FE4D
#define HF7A8BCE1_C419_424E_B625_A47D5C30FE4D

#include <state/l4.h>
#include <trans-dsl/action/SimpleAsyncActionHelper.h>

L4_NS_BEGIN

DEF_SIMPLE_ASYNC_ACTION(FooAsynAction4)
{
    OVERRIDE(cub::Status exec(const tsl::TransactionInfo&));

private:
    cub::Status handleEvent4(const tsl::TransactionInfo&, const ev::Event&);
};

L4_NS_END

#endif /* HF7A8BCE1_C419_424E_B625_A47D5C30FE4D */
