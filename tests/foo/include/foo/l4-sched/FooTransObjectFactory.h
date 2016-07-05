#ifndef H12A546CE_E942_40CC_8573_6B0AF9CAEC7E
#define H12A546CE_E942_40CC_8573_6B0AF9CAEC7E

#include <state/StateId.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

struct UnstableState;

struct FooTransObjectFactory
{
    static UnstableState* createState(const tsl::InstanceId, const StateId);
};

L4_NS_END

#endif /* H12A546CE_E942_40CC_8573_6B0AF9CAEC7E */
