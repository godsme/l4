#include <foo/l4-sched/FooBufferedEventFactory.h>
#include <cub/mem/ObjectAllocator.h>
#include <event/concept/Event.h>
#include <state/BlockBufferedEventInfo.h>

L4_NS_BEGIN

using namespace cub;
using namespace ev;

namespace
{
    template<typename T, U16 NUM>
    struct BuffMessage
    {
        void* operator new(size_t) throw()
        {
            return allocator.alloc();
        }

        void operator delete(void* p)
        {
            allocator.free(p);
        }

        static bool within(void* p)
        {
            return allocator.within(p);
        }

    private:
        typedef ObjectAllocator<T, NUM> Allocator;
        static Allocator allocator;
    };

    template<typename T, U16 NUM>
    typename BuffMessage<T, NUM>::Allocator BuffMessage<T, NUM>::allocator;

    template<U16 BUFFER_SIZE, U16 NUM>
    struct GenericBlockBufferedEventInfoWithAllocator : GenericBlockBufferedEventInfo<BUFFER_SIZE>
                                                      , BuffMessage<GenericBlockBufferedEventInfo<BUFFER_SIZE>, NUM>
    {
        GenericBlockBufferedEventInfoWithAllocator( const TransStrategyDecisionMaker& strategy
        , const FailedRequestListener* listener)
        : GenericBlockBufferedEventInfo<BUFFER_SIZE>(strategy, listener)
        {
            static_assert(sizeof(*this) == sizeof(GenericBlockBufferedEventInfo<BUFFER_SIZE>),
                     "class GenericBlockBufferedEventInfoWithAllocator should not change object size");
        }

        static bool withInBufferSize(size_t size)
        {
            return size <= BUFFER_SIZE;
        }
    };


#ifdef _MSC_VER
    typedef GenericBlockBufferedEventInfoWithAllocator<1, 1200> BlockEmptyEvent;
#else
    typedef GenericBlockBufferedEventInfoWithAllocator<0, 1200> BlockEmptyEvent;
#endif

    // Configure different size & number of blocks.
    typedef GenericBlockBufferedEventInfoWithAllocator<64, 128> Block64BEvent;
    typedef GenericBlockBufferedEventInfoWithAllocator<13 * 1024, 20> Block13KBEvent;
    typedef GenericBlockBufferedEventInfoWithAllocator<40 * 1024, 10> Block40KBEvent;
}

////////////////////////////////////////////////////////////////////////////
FooBufferedEventFactory& FooBufferedEventFactory::getInstance()
{
   static FooBufferedEventFactory instance;
   return instance;
}

////////////////////////////////////////////////////////////////////////////
BufferedEventInfo*
FooBufferedEventFactory::createBufferedEvent(const Event& event,
         TransStrategyDecisionMaker& strategy, FailedRequestListener* listener)
{
    size_t size = event.getMsgSize();
    if(BlockEmptyEvent::withInBufferSize(size))
        return new BlockEmptyEvent(strategy, listener);
    if(Block64BEvent::withInBufferSize(size))
        return new Block64BEvent(strategy, listener);
    if(Block13KBEvent::withInBufferSize(size))
        return new Block13KBEvent(strategy, listener);
    if(Block40KBEvent::withInBufferSize(size))
        return new Block40KBEvent(strategy, listener);

    return (BufferedEventInfo*)0;
}

//////////////////////////////////////////////////////////////////////////
bool FooBufferedEventFactory::isBufferedEvent(const EventInfo& info) const
{
    void* p = (void*)&info;
    return BlockEmptyEvent::within(p)
        || Block64BEvent::within(p)
        || Block13KBEvent::within(p)
        || Block40KBEvent::within(p);
}

L4_NS_END
