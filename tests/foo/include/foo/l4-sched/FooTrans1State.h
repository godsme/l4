#ifndef H06DC3D20_D9BF_4A15_A294_FFC92F7826E9
#define H06DC3D20_D9BF_4A15_A294_FFC92F7826E9

#include <foo/l3-trans/FooTrans1Context.h>
#include <foo/l4-sched/FooGenericTransaction.h>
#include <foo/l4-sched/FooTrans1State.h>
#include <trans-dsl/sched/concept/TransactionListener.h>

L4_NS_BEGIN

struct FooTrans1State
        : FooGenericTransaction<FooTrans1, FooTrans1Context, tsl::TransactionListener>
{
};

L4_NS_END

#endif /* H06DC3D20_D9BF_4A15_A294_FFC92F7826E9 */
