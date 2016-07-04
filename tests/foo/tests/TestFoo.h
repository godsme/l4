
#include <testngpp/testngpp.hpp>
#include <event/concept/Event.h>
#include <event/impl/SimpleEventInfo.h>
#include <foo/l4-sched/FooEvent.h>
#include <foo/l4-sched/FooStateMachine.h>

#include <iostream>

using namespace l4;
using namespace ev;

USING_TESTNGPP_NS;

#define EVENT(n) SimpleEventInfo(n)

FIXTURE(Foo)
{
    FooStateMachine sm{0};

    SETUP()
    {
        ASSERT_EQ(TSL_CONTINUE, sm.start());
    }

    TEST("should return UNKNOWN_EVENT when recv a unexpected event")
    {
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT2)));
    }

    // @test(id="ev-1")
    TEST("should consume expected event")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT1)));
    }

    // @test(depends="ev-1")
    TEST("should consume the next expected event")
    {

    }
};



