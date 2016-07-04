#include <foo/l4-sched/FooStateMachine.h>
#include <foo/l4-sched/FooBufferedEventFactory.h>

L4_NS_BEGIN

FooStateMachine::FooStateMachine(tsl::InstanceId iid)
    : EventQueue(FooBufferedEventFactory::getInstance())
    , TransStateMachine(*this, *this, iid)
{
}

L4_NS_END
