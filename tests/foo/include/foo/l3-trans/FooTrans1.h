#ifndef HDA721C5C_73E8_411C_B459_F7F47C2573C3
#define HDA721C5C_73E8_411C_B459_F7F47C2573C3

#include <state/l4.h>
#include <trans-dsl/TransactionDsl.h>

L4_NS_BEGIN

__transaction(FooTrans1)
( __sequential
    ( __asyn(FooAsynAction1)
    , __asyn(FooAsynAction2)));

L4_NS_END

#endif /* HDA721C5C_73E8_411C_B459_F7F47C2573C3 */
