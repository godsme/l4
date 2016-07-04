#ifndef HAF238781_B7ED_44E5_BE66_4D3F36AC8CD0
#define HAF238781_B7ED_44E5_BE66_4D3F36AC8CD0

#include <state/l4.h>
#include <trans-dsl/action/SimpleAsyncActionHelper.h>

L4_NS_BEGIN

DEF_SIMPLE_ASYNC_ACTION(FooAsynAction2)
{
    OVERRIDE(cub::Status exec(const tsl::TransactionInfo&));

private:
    cub::Status handleEvent2(const tsl::TransactionInfo&, const ev::Event&);
};

L4_NS_END

#endif /* HAF238781_B7ED_44E5_BE66_4D3F36AC8CD0 */
