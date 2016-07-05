#ifndef H98F2DCCE_CB30_4664_92F9_35C8A4A39590
#define H98F2DCCE_CB30_4664_92F9_35C8A4A39590

#include <foo/l2-context/FooAsynAction4.h>
#include <foo/l2-context/FooAsynAction5.h>
#include <foo/l2-context/FooSyncAction3.h>
#include <foo/l3-trans/FooGenericTransaction.h>
#include <foo/l3-trans/trans2/FooTrans2Context.h>
#include <trans-dsl/sched/concept/TransactionListener.h>
#include <trans-dsl/TransactionDsl.h>


L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
__def_transaction
( __sequential
    ( __asyn(FooAsynAction4)
    , __asyn(FooAsynAction5)
    , __sync(FooSyncAction3))
, __finally(__on_fail(__throw(TSL_NOTHING_CHANGED)))
) FooTrans2;

///////////////////////////////////////////////////////////////////
__def_pack_trans(FooTrans2, FooTrans2Context) FooTrans2Trans;

L4_NS_END

#endif /* H98F2DCCE_CB30_4664_92F9_35C8A4A39590 */
