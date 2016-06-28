#ifndef HB7FB23C7_0014_402E_80C4_CDD2A6DB0CF8
#define HB7FB23C7_0014_402E_80C4_CDD2A6DB0CF8

#include <state/l4.h>

L4_NS_BEGIN

//////////////////////////////////////////////////////////////////////////////
enum : unsigned char
{
    __TS_KEEP_OLD   = 0x01,
    __TS_STOP_OLD   = 0x02,
    __TS_KILL_OLD   = 0x04,
    __TS_SUSP_OLD   = 0x08,

    __TS_BUFF_NEW   = 0x01,
    __TS_DENY_NEW   = 0x02,
    __TS_DROP_NEW   = 0x03,
    __TS_EXEC_NEW   = 0x04,

    __TS_NORM_SCHED = 0,
    __TS_FAIL_SCHED = 1
};

//////////////////////////////////////////////////////////////////////////////
#define __TS_DEF(ts_old, ts_new, ts_sched) \
   (__TS_##ts_old << 8) | (__TS_##ts_new << 1) | (__TS_##ts_sched)


//////////////////////////////////////////////////////////////////////////////
enum TransStrategy : unsigned short
{
    TS_DISCARD            = __TS_DEF(KEEP_OLD, DROP_NEW, NORM_SCHED),
    TS_REJECT             = __TS_DEF(KEEP_OLD, DENY_NEW, NORM_SCHED),
    TS_BUFFER             = __TS_DEF(KEEP_OLD, BUFF_NEW, NORM_SCHED),
    TS_INTERRUPT          = __TS_DEF(STOP_OLD, DROP_NEW, NORM_SCHED),
    TS_INTERRUPT_FAIL     = __TS_DEF(STOP_OLD, DROP_NEW, FAIL_SCHED),

    TS_PREEMPT            = __TS_DEF(STOP_OLD, BUFF_NEW, NORM_SCHED),
    TS_REPLACE            = __TS_DEF(KILL_OLD, EXEC_NEW, NORM_SCHED),
    TS_FAIL               = __TS_DEF(KILL_OLD, DROP_NEW, FAIL_SCHED),
    TS_REJ_FAIL           = __TS_DEF(KILL_OLD, DENY_NEW, FAIL_SCHED),
    TS_REJ_INTERRUPT      = __TS_DEF(STOP_OLD, DENY_NEW, NORM_SCHED),
    TS_SUSPEND            = __TS_DEF(SUSP_OLD, EXEC_NEW, NORM_SCHED),
    TS_INTERRUPT_SUSPEND  = __TS_DEF(STOP_OLD, EXEC_NEW, NORM_SCHED),

    TS_NIL                = 0,
    TS_UNKNOWN            = 0xFFFF,
};

//////////////////////////////////////////////////////////////////////////

L4_NS_END

#endif /* HB7FB23C7_0014_402E_80C4_CDD2A6DB0CF8 */
