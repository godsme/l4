#ifndef H87056ADD_283D_4836_8BD7_F9074976CCF1
#define H87056ADD_283D_4836_8BD7_F9074976CCF1

#include <state/l4.h>
#include <trans-dsl/ext/mutex/concept/TransMutextId.h>
#include <trans-dsl/ext/mutex/impl/TransMutexAvailNotifier.h>

L4_NS_BEGIN

struct SimpleTransAvailMutex : tsl::TransMutexAvailNotifier
{
    SimpleTransAvailMutex();

    tsl::TransMutexId fetchAailMutex()
    {
        if(availMutex == 0) return 0xFF;

        return doFetchAailMutex();
    }

private:
    OVERRIDE(void notifyMutexAvail(tsl::TransMutexId));
    tsl::TransMutexId doFetchAailMutex();

protected:
    cub::U8 availMutex;
};

L4_NS_END

#endif /* H87056ADD_283D_4836_8BD7_F9074976CCF1 */
