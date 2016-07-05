#ifndef HDD7A70CD_6E53_4FF5_8D88_CE726E8D3C1E
#define HDD7A70CD_6E53_4FF5_8D88_CE726E8D3C1E

#include <trans-dsl/sched/trans/GenericTransaction.h>
#include <foo/l1-domain/FooTimerInfo.h>

L4_NS_BEGIN

template <typename TRANS, typename CONTEXT, typename LISTENER>
struct FooGenericTransaction
   : tsl::GenericTransaction<FooTimerInfo, TRANS, CONTEXT, LISTENER>
{
private:
    typedef tsl::GenericTransaction<FooTimerInfo, TRANS, CONTEXT, LISTENER> Base;

public:
    FooGenericTransaction(tsl::InstanceId iid)
        : Base(iid)
    {}
};

L4_NS_END

#endif /* HDD7A70CD_6E53_4FF5_8D88_CE726E8D3C1E */
