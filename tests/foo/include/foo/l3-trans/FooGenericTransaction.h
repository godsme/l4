#ifndef HDD7A70CD_6E53_4FF5_8D88_CE726E8D3C1E
#define HDD7A70CD_6E53_4FF5_8D88_CE726E8D3C1E

#include <trans-dsl/sched/trans/GenericTransaction.h>
#include <foo/l1-domain/FooTimerInfo.h>
#include <trans-dsl/sched/concept/TransactionListener.h>

L4_NS_BEGIN

template <typename TRANS, typename CONTEXT, typename LISTENER = tsl::TransactionListener>
struct FooGenericTransaction
   : tsl::GenericTransaction<FooTimerInfo, TRANS, CONTEXT, LISTENER>
{
    FooGenericTransaction(tsl::InstanceId iid)
        : tsl::GenericTransaction<FooTimerInfo, TRANS, CONTEXT, LISTENER>(iid)
    {}
};

#define __def_pack_trans(trans, context, ...) typedef FooGenericTransaction<trans, context, ##__VA_ARGS__>

L4_NS_END

#endif /* HDD7A70CD_6E53_4FF5_8D88_CE726E8D3C1E */
