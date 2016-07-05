#ifndef H25B914A4_3E80_4B9F_AF78_9D9A991663E8
#define H25B914A4_3E80_4B9F_AF78_9D9A991663E8

#include <foo/l2-context/FooAsynAction6.h>
#include <foo/l2-context/FooAsynActionR.h>
#include <foo/l2-context/FooSyncAction3.h>
#include <foo/l3-trans/FooGenericTransaction.h>
#include <foo/l3-trans/release/FooReleaseContext.h>
#include <trans-dsl/sched/concept/TransactionListener.h>
#include <trans-dsl/TransactionDsl.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
__def_transaction
( __sequential
    ( __asyn(FooAsynActionR)
    , __sync(FooSyncAction3)
    , __asyn(FooAsynAction6))
) FooRelease;

///////////////////////////////////////////////////////////////////
__def_pack_trans(FooRelease, FooReleaseContext) FooReleaseTrans;
///////////////////////////////////////////////////////////////////

L4_NS_END

#endif /* H25B914A4_3E80_4B9F_AF78_9D9A991663E8 */
