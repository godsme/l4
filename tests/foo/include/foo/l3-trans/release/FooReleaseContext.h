#ifndef HC2841219_4E95_48B5_93E8_C668D45C03AC
#define HC2841219_4E95_48B5_93E8_C668D45C03AC

#include <cub/dci/Unknown.h>
#include <foo/l3-trans/common/Context0.h>
#include <foo/l3-trans/release/Context4.h>

L4_NS_BEGIN

struct FooReleaseContext
        : com::Unknown
        , Context0
        , Context4
{
    BEGIN_INTERFACE_TABLE()
    __HAS_INTERFACE(Context0)
    __HAS_INTERFACE(Context4)
    END_INTERFACE_TABLE()
};

L4_NS_END

#endif /* HC2841219_4E95_48B5_93E8_C668D45C03AC */
