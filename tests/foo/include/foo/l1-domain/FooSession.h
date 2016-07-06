#ifndef H7C736F84_E3C0_4462_BF40_C96BBB47E2B8
#define H7C736F84_E3C0_4462_BF40_C96BBB47E2B8

#include <cub/base/Status.h>
#include <cub/dci/Role.h>
#include <state/l4.h>

L4_NS_BEGIN


DEFINE_ROLE(FooSession)
{
    FooSession(cub::U32 id)
        : id(id)
    {}

    cub::U32 getId() const
    {
        return id;
    }

    void foo();

private:
    cub::U32 id;
};

L4_NS_END

#endif /* H7C736F84_E3C0_4462_BF40_C96BBB47E2B8 */
