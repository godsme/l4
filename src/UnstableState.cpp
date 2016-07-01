#include <state/UnstableState.h>
#include <trans-dsl/utils/ActionStatus.h>

L4_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
UnstableState::UnstableState()
   : from(STATE_NIL)
{
}

//////////////////////////////////////////////////////////////////////////
void UnstableState::startFrom(const StateId from)
{
    this->from = from;
}

//////////////////////////////////////////////////////////////////////////
bool UnstableState::isStable() const
{
   return false;
}

//////////////////////////////////////////////////////////////////////////
cub::Status UnstableState::enter(StateMachine& context)
{
   return finalProcess(context, doEnter());
}

//////////////////////////////////////////////////////////////////////////
cub::Status UnstableState::handleEvent(StateMachine& context, const ev::Event& event)
{
   return finalProcess(context, doHandleEvent(event));
}

//////////////////////////////////////////////////////////////////////////
cub::Status UnstableState::leave(StateMachine& context, const cub::Status cause)
{
   return finalProcess(context, doLeave(cause));
}

//////////////////////////////////////////////////////////////////////////
void UnstableState::kill(StateMachine& context, const cub::Status cause)
{
    doKill(cause);
}

//////////////////////////////////////////////////////////////////////////
cub::Status UnstableState::finalProcess(StateMachine& context, const tsl::ActionStatus& result)
{
    if(result.isDone())
    {
        context.transitTo(getToState());
        return TSL_SUCCESS;
    }

    if(result.isNothingChanged())
    {
        context.transitTo(from);
        return TSL_SUCCESS;
    }

    return result;
}

L4_NS_END
