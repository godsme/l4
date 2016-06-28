#ifndef H89E5BB7A_2737_4810_8CB9_CAD7D89DD571
#define H89E5BB7A_2737_4810_8CB9_CAD7D89DD571

#include <state/l4.h>
#include <cub/dci/Role.h>
#include <event/concept/EventId.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
DEFINE_ROLE(TransMsgDiscarder)
{
    void discard(ev::EventId eventId)
    {
        doDiscard(eventId);
    }

private:
    ABSTRACT(void doDiscard(ev::EventId eventId));
};

///////////////////////////////////////////////////////////////////
#define  DECL_TRANS_EVNET_DEQUEUE()                             \
    private:                                                    \
const TransMsgDiscard* discard;                                 \
public:                                                         \
    void setTransMachineDiscard(const TransMsgDiscard* discard) \
    {                                                           \
        this->discard = discard;                                \
    }                                                           \
    const TransMsgDiscard* getTransMachineDiscard() const       \
    {                                                           \
        return discard;                                         \
    }


L4_NS_END

#endif /* H89E5BB7A_2737_4810_8CB9_CAD7D89DD571 */
