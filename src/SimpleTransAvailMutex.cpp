#include <state/SimpleTransAvailMutex.h>

L4_NS_BEGIN

using namespace tsl;

SimpleTransAvailMutex::SimpleTransAvailMutex()
    : availMutex(0)
{
}

void SimpleTransAvailMutex::notifyMutexAvail(TransMutexId mutex)
{
    availMutex |= (cub::U8)(1<<mutex);
}

TransMutexId SimpleTransAvailMutex::doFetchAailMutex()
{
    for(int i=0; i<8; i++)
    {
        if(availMutex & (cub::U8)(1<<i))
        {
            availMutex &= (cub::U8)~(1<<i);
            return i;
        }
    }

    return 0xFF;
}

L4_NS_END
