
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
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT1_T)));
    }

    // @test(depends="ev-1")
    TEST("should discard event 1")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT1_T)));
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT2)));
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT2)));
    }

    // @test(depends="ev-1")
    TEST("should buffer event 4")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT4_T)));
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT4_T)));
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT2)));
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT5)));
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT5)));
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT5)));
    }

    // @test(depends="ev-1")
    TEST("should preempt event R")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT_R)));
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT2)));
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT6)));
    }

    // @test(id="ev-2", depends="ev-1")
    TEST("should consume the next expected event")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT2)));
    }

    // @test(depends="ev-2")
    TEST("should reject unexpected event")
    {
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT2)));
    }

    // @test(id="ev-4", depends="ev-2")
    TEST("should start new trans")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT4_T)));
    }

    // @test(depends="ev-4")
    TEST("should reject unexpected event")
    {
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT2)));
    }

    // @test(id="ev-5", depends="ev-4")
    TEST("should consume the next expected event")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT5)));
    }

    // @test(id="ev-4-2", depends="ev-5")
    TEST("should start new trans")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT4_T)));
    }

    // @test(id="ev-5-2", depends="ev-4-2")
    TEST("should consume the next expected event")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT5)));
    }

    // @test(depends="ev-5-2")
    TEST("should reject event-1 on Active state")
    {
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT1_T)));
    }

    // @test(depends="ev-5-2")
    TEST("should reject event-2 on Active state")
    {
        ASSERT_EQ(TSL_UNKNOWN_EVENT, sm.handleEvent(EVENT(EV_EVENT2)));
    }

    // @test(id="rel", depends="ev-5-2")
    TEST("should start release trans when recv EV_EVENT_R on action state")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT_R)));
    }

    // @test(depends="rel")
    TEST("should be able to release")
    {
        ASSERT_EQ(TSL_CONTINUE, sm.handleEvent(EVENT(EV_EVENT6)));
    }

};



