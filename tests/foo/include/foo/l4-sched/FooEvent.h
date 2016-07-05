#ifndef HFBDA8972_5F46_4E17_A412_03A909F0057F
#define HFBDA8972_5F46_4E17_A412_03A909F0057F

#include <event/concept/EventId.h>
#include <state/l4.h>

L4_NS_BEGIN

enum : ev::EventId
{
    EV_EVENT1_T,
    EV_EVENT2,
    EV_EVENT4_T,
    EV_EVENT5,
    EV_EVENT_R,
    EV_EVENT6
};

struct Event1
{
    int a, b;
};

struct Event2
{
    bool c, d;
};

struct Event4
{
    float c, d;
};

L4_NS_END

#endif /* HFBDA8972_5F46_4E17_A412_03A909F0057F */
