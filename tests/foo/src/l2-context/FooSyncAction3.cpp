#include <foo/l2-context/FooSyncAction3.h>
#include <trans-dsl/TslStatus.h>
#include <cub/log/log.h>

L4_NS_BEGIN

DEF_ACTION_EXEC(FooSyncAction3)
{
    return TSL_SUCCESS;
}

L4_NS_END
