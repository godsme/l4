#include <cub/base/Assertions.h>
#include <state/BlockBufferedEventInfo.h>
#include <trans-dsl/TslStatus.h>
#include <event/concept/InvalidMsgSize.h>

#include <string.h>

L4_NS_BEGIN

using namespace cub;
using namespace ev;

//////////////////////////////////////////////////////////////////
BlockBufferedEventInfo::BlockBufferedEventInfo
     ( const TransStrategyDecisionMaker& strategy
     , const FailedRequestListener* listener)
   : BufferedEventInfo(&strategy, listener)
   , size(INVALID_MSG_SIZE)
{
}

//////////////////////////////////////////////////////////////////
const void* BlockBufferedEventInfo::getMsg() const
{
   return getMsgBuffer();
}

//////////////////////////////////////////////////////////////////
Status BlockBufferedEventInfo::saveEvent(const EventInfo& event)
{
   CUB_ASSERT_SUCC_CALL(saveMsg(event));
   updateEventId(event.getEventId());
   return TSL_SUCCESS;
}

//////////////////////////////////////////////////////////////////
const void* BlockBufferedEventInfo::getMsgBuffer() const
{
   return getMsgBlock(size);
}

//////////////////////////////////////////////////////////////////
size_t BlockBufferedEventInfo::getMsgSize() const
{
   return size;
}

//////////////////////////////////////////////////////////////////
Status BlockBufferedEventInfo::saveMsg(const EventInfo& event)
{
   const void* const src = event.getMsg();
   if (src == 0)
   {
      if(event.getMsgSize() != 0) return TSL_FAILED;

      size = 0;

      return TSL_SUCCESS;
   }

   const void* dest = getMsgBlock(event.getMsgSize());
   CUB_ASSERT_VALID_PTR(dest);

   size = event.getMsgSize();
   ::memcpy((void*) dest, src, size);

   return TSL_SUCCESS;
}

L4_NS_END
