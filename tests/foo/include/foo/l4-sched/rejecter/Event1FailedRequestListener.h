#ifndef HC80CDE64_1BD5_461E_ACCE_B2AE8F891AF1
#define HC80CDE64_1BD5_461E_ACCE_B2AE8F891AF1

#include <cub/log/log.h>
#include <event/concept/Event.h>
#include <state/FailedRequestListener.h>

L4_NS_BEGIN

namespace details
{
    struct Event1FailedRequestListener
    {
        static void onReject(const tsl::InstanceId iid, const ev::Event& event, const StateId sid)
        {
            WARN_LOG("reject %d on state %d", event.getEventId(), sid);
        }

        static void onDiscard(const tsl::InstanceId iid, const ev::Event& event, const StateId sid)
        {
            WARN_LOG("discard %d on state %d", event.getEventId(), sid);
        }
    };
}

typedef GenericFailedRequestListener<details::Event1FailedRequestListener> Event1Rejector;

L4_NS_END

#endif /* HC80CDE64_1BD5_461E_ACCE_B2AE8F891AF1 */
