#ifndef HBF780164_0247_417B_A03C_967F0BF77D70
#define HBF780164_0247_417B_A03C_967F0BF77D70

#include <cub/dci/Unknown.h>
#include <foo/l3-trans/common/Context0.h>
#include <foo/l3-trans/trans2/Context3.h>

L4_NS_BEGIN

struct FooTrans2Context
        : com::Unknown
        , Context0
        , Context3
{
    BEGIN_INTERFACE_TABLE()
    __HAS_INTERFACE(Context0)
    __HAS_INTERFACE(Context3)
    END_INTERFACE_TABLE()
};

L4_NS_END
#endif /* HBF780164_0247_417B_A03C_967F0BF77D70 */
