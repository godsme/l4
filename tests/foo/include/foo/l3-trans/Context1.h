#ifndef H712DC006_B0A4_45C9_B447_D1C1FA9AC7CE
#define H712DC006_B0A4_45C9_B447_D1C1FA9AC7CE

#include <state/l4.h>
#include <cub/dci/InterfaceDef.h>

L4_NS_BEGIN

UNKNOWN_INTERFACE(Context1, 0x08311757)
{
    int getValue()
    {
        return 5;
    }
};

L4_NS_END

#endif /* H712DC006_B0A4_45C9_B447_D1C1FA9AC7CE */
