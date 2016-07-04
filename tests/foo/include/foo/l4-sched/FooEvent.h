#ifndef HFBDA8972_5F46_4E17_A412_03A909F0057F
#define HFBDA8972_5F46_4E17_A412_03A909F0057F

#include <event/concept/EventId.h>
#include <state/l4.h>

L4_NS_BEGIN

enum : ev::EventId
{
    EV_EVENT1,
    EV_EVENT2
};

struct Event1
{
    int a, b;
};

struct Event2
{
    int c, d;
};

L4_NS_END

#endif /* HFBDA8972_5F46_4E17_A412_03A909F0057F */
