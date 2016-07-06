#ifndef HB6F66424_1F7C_4436_B641_9A4C22F10BD3
#define HB6F66424_1F7C_4436_B641_9A4C22F10BD3

#include <state/l4.h>
#include <cub/dci/Role.h>
#include <event/event.h>

FWD_DECL_EV(Event);
FWD_DECL_EV(EventInfo);

L4_NS_BEGIN

struct BufferedEventInfo;
struct TransStrategyDecisionMaker;
struct FailedRequestListener;

DEFINE_ROLE(BufferedEventFactory)
{
   ABSTRACT(BufferedEventInfo* createBufferedEvent
        (const ev::Event&, const TransStrategyDecisionMaker&, FailedRequestListener*));

   ABSTRACT(bool isBufferedEvent(const ev::EventInfo&) const);
};


L4_NS_END

#endif /* HB6F66424_1F7C_4436_B641_9A4C22F10BD3 */
