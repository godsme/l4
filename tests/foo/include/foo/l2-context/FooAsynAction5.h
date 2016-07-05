#ifndef H63076A3F_E8BA_4674_B4C3_6A5D93BA8D72
#define H63076A3F_E8BA_4674_B4C3_6A5D93BA8D72

#include <state/l4.h>
#include <trans-dsl/action/SimpleAsyncActionHelper.h>

L4_NS_BEGIN

DEF_SIMPLE_ASYNC_ACTION(FooAsynAction5)
{
    OVERRIDE(cub::Status exec(const tsl::TransactionInfo&));

private:
    cub::Status handleEvent5(const tsl::TransactionInfo&, const ev::Event&);
};

L4_NS_END

#endif /* H63076A3F_E8BA_4674_B4C3_6A5D93BA8D72 */
