#ifndef H0DDCDDE8_813C_45C4_BBC0_8C725E7910E2
#define H0DDCDDE8_813C_45C4_BBC0_8C725E7910E2

#include <cub/dci/Unknown.h>
#include <foo/l3-trans/Context1.h>
#include <foo/l3-trans/Context2.h>
#include <state/l4.h>

L4_NS_BEGIN

struct FooTrans1Context
        : com::Unknown
        , Context1
        , Context2
{
    BEGIN_INTERFACE_TABLE()
    __HAS_INTERFACE(Context1)
    __HAS_INTERFACE(Context2)
    END_INTERFACE_TABLE()
};

L4_NS_END

#endif /* H0DDCDDE8_813C_45C4_BBC0_8C725E7910E2 */
