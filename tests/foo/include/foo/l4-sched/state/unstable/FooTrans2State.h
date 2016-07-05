#ifndef HEA99ABA8_6237_4C01_9B24_A29C16925338
#define HEA99ABA8_6237_4C01_9B24_A29C16925338

#include <foo/l3-trans/trans2/FooTrans2.h>
#include <foo/l4-sched/FooStateId.h>
#include <foo/l4-sched/state/unstable/FooTrans1State.h>
#include <state/GenericUnstableState.h>
#include <state/UnstableTransState.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <trans-dsl/sched/concept/TransactionListener.h>

L4_NS_BEGIN


///////////////////////////////////////////////////////////////////////////////////////////////////
struct FooTrans2StaticStrategy
{
    static TransStrategy getStrategy(const ev::Event&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct FooTrans2RuntimeStrategy
{
    static TransStrategy getStrategy(tsl::InstanceId, com::Unknown*, const ev::Event&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
typedef GenericUnstableState
           < STATE_Trans2
           , STATE_Active
           , FooTrans2Trans
           , FooTrans2StaticStrategy
           , FooTrans2RuntimeStrategy> FooTrans2State;


L4_NS_END


#endif /* HEA99ABA8_6237_4C01_9B24_A29C16925338 */
