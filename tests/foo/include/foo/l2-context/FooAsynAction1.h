#ifndef H9D64045D_47AA_41F4_8A1A_FAB9AE864BB3
#define H9D64045D_47AA_41F4_8A1A_FAB9AE864BB3

#include <state/l4.h>
#include <trans-dsl/action/SimpleAsyncActionHelper.h>

L4_NS_BEGIN

DEF_SIMPLE_ASYNC_ACTION(FooAsynAction1)
{
    OVERRIDE(cub::Status exec(const tsl::TransactionInfo&));

private:
    cub::Status handleEvent1(const tsl::TransactionInfo&, const ev::Event&);
};

L4_NS_END

#endif /* H9D64045D_47AA_41F4_8A1A_FAB9AE864BB3 */
