#ifndef H5A79F452_73FE_4299_BD4A_2E7066EA49AC
#define H5A79F452_73FE_4299_BD4A_2E7066EA49AC

#include <state/BufferedEventFactory.h>

L4_NS_BEGIN

struct FooBufferedEventFactory : BufferedEventFactory
{
   static FooBufferedEventFactory& getInstance();

   OVERRIDE(BufferedEventInfo*
   createBufferedEvent(const ev::Event&, const TransStrategyDecisionMaker&, FailedRequestListener*));

   OVERRIDE(bool isBufferedEvent(const ev::EventInfo&) const);
};

L4_NS_END

#endif /* H5A79F452_73FE_4299_BD4A_2E7066EA49AC */
