#ifndef H3000CA41_45DE_4050_8B11_89A9135EE0D5
#define H3000CA41_45DE_4050_8B11_89A9135EE0D5

#include <foo/l3-trans/release/FooRelease.h>
#include <foo/l4-sched/FooStateId.h>
#include <foo/l4-sched/state/unstable/FooTrans1State.h>
#include <state/DefaultTransStrategy.h>
#include <state/GenericUnstableState.h>
#include <state/UnstableTransState.h>
#include <trans-dsl/sched/concept/InstanceId.h>
#include <trans-dsl/sched/concept/TransactionListener.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////////////////////////////////////
typedef GenericUnstableState
           < STATE_Release
           , STATE_Idle
           , FooReleaseTrans
           , RejectAllStaticStrategy
           , RejectAllRuntimeStrategy> FooReleaseState;


L4_NS_END

#endif /* H3000CA41_45DE_4050_8B11_89A9135EE0D5 */
