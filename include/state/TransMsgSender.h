#ifndef HE4AAA9CC_3F59_48E7_B416_502599EC5CEA
#define HE4AAA9CC_3F59_48E7_B416_502599EC5CEA

#include <state/l4.h>
#include <cub/dci/Role.h>
#include <event/concept/EventId.h>
#include <event/event.h>

FWD_DECL_EV(Event);

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
DEFINE_ROLE(TransMsgSender)
{
    cub::Status send(ev::EventId eventId);

    template <typename MSG_TYPE>
    cub::Status send(ev::EventId eventId, const MSG_TYPE& msg)
    {
        return doBuffer(ConsecutiveEventInfo(eventId, msg));
    }

    cub::Status send(const ev::Event& event)
    {
        return doBuffer(event);
    }

private:
    ABSTRACT(cub::Status doBuffer(const ev::Event& event));
};

///////////////////////////////////////////////////////////////////
#define  DECL_TRANS_EVENT_INQUEUE()                         \
    private:                                                \
const TransMsgSender* sender;                               \
public:                                                     \
    void setTransMachineSender(const TransMsgSender* sender)\
    {                                                       \
        this->sender = sender;                              \
    }                                                       \
    const TransMsgSender* getTransMachineSender() const     \
    {                                                       \
        return sender;                                      \
    }


L4_NS_END

#endif /* HE4AAA9CC_3F59_48E7_B416_502599EC5CEA */
