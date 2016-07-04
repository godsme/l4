
#include <state/UnstableTransState.h>
#include <trans-dsl/sched/concept/Transaction.h>
L4_NS_BEGIN

using namespace cub;

///////////////////////////////////////////////////////////////////
Status UnstableTransState::doEnter()
{
    return ROLE(Transaction).start();
}

///////////////////////////////////////////////////////////////////
Status UnstableTransState::doHandleEvent(const ev::Event& event)
{
    Status status = ROLE(Transaction).handleEvent(event);
    if(status != TSL_UNKNOWN_EVENT)
    {
        return status;
    }

    return postprocess(event);
}

///////////////////////////////////////////////////////////////////
Status UnstableTransState::doLeave(const Status cause)
{
    return ROLE(Transaction).stop(cause);
}

void UnstableTransState::doKill(const Status cause)
{
    ROLE(Transaction).kill(cause);
}

Status UnstableTransState::postprocess(const ev::Event& event)
{
    Status status = postTransaction(event);
    if(__CUB_FAILED(status))
    {
        ROLE(Transaction).kill(status);
    }

    return status == TSL_SUCCESS ? TSL_CONTINUE : status;
}

//////////////////////////////////////////////////////////////////////////
Status UnstableTransState::postTransaction(const ev::Event&)
{
    return TSL_UNKNOWN_EVENT;
}

//////////////////////////////////////////////////////////////////////////
TransStrategy UnstableTransState::getRuntimeStrategy(const tsl::InstanceId, const ev::Event&) const
{
    return TS_UNKNOWN;
}

L4_NS_END
