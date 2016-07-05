#ifndef HC9B7A623_3E4B_4C32_82E3_8B3514DF2BC0
#define HC9B7A623_3E4B_4C32_82E3_8B3514DF2BC0

#include <state/l4.h>
#include <cub/dci/InterfaceDef.h>

L4_NS_BEGIN

UNKNOWN_INTERFACE(Context2, 0x08311758)
{
    int getValue()
    {
        return 10;
    }
};

L4_NS_END

#endif /* HC9B7A623_3E4B_4C32_82E3_8B3514DF2BC0 */
