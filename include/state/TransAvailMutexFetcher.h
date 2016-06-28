#ifndef H8A36D7C4_4589_47E6_A4E3_33DF1E3C64FA
#define H8A36D7C4_4589_47E6_A4E3_33DF1E3C64FA

#include <state/l4.h>
#include <cub/dci/Role.h>
#include <trans-dsl/ext/mutex/concept/TransMutextId.h>

L4_NS_BEGIN

DEFINE_ROLE(TransAvailMutexFetcher)
{
    ABSTRACT(tsl::TransMutexId fetchAailMutex());
};

L4_NS_END

#endif /* H8A36D7C4_4589_47E6_A4E3_33DF1E3C64FA */
