#include <foo/l1-domain/FooTimerInfo.h>

L4_NS_BEGIN

cub::U32 FooTimerInfo::getTimerLen(const tsl::TimerId) const
{
    return 0;
}

L4_NS_END
