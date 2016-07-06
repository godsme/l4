#ifndef H57969C77_F822_4354_965C_C68A7BE61F38
#define H57969C77_F822_4354_965C_C68A7BE61F38

#include <foo/l1-domain/FooSession.h>
#include <foo/l4-sched/FooStateMachine.h>
#include <trans-dsl/sched/concept/InstanceId.h>

L4_NS_BEGIN

struct FooObject
        : FooSession
        , FooStateMachine
{
    FooObject(cub::U32 id)
        : FooSession(id)
        , FooStateMachine(reinterpret_cast<tsl::InstanceId>(this))
    {}
};

L4_NS_END

#endif /* H57969C77_F822_4354_965C_C68A7BE61F38 */
