#ifndef H12A546CE_E942_40CC_8573_6B0AF9CAEC7E
#define H12A546CE_E942_40CC_8573_6B0AF9CAEC7E

#include <event/concept/EventId.h>
#include <state/StateId.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

struct UnstableStateInfo;

struct FooUnstableStateInfoRepo
{
    static UnstableStateInfo& getUnstableInfoByEvent(ev::EventId);
};

L4_NS_END

#endif /* H12A546CE_E942_40CC_8573_6B0AF9CAEC7E */
