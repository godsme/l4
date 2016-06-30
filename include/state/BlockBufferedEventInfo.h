#ifndef HD5783E6C_4701_4324_9769_CAE7C24B8376
#define HD5783E6C_4701_4324_9769_CAE7C24B8376

#include <state/BufferedEventInfo.h>

L4_NS_BEGIN

///////////////////////////////////////////////////////////////////
struct BlockBufferedEventInfo: BufferedEventInfo
{
   BlockBufferedEventInfo( const TransStrategyDecisionMaker& strategy
                         , const FailedRequestListener* listener);

   OVERRIDE(const void* getMsg() const);
   OVERRIDE(const void* getMsgBuffer() const);
   OVERRIDE(size_t getMsgSize() const);

   OVERRIDE(cub::Status saveEvent(const EventInfo& event));

private:
   cub::Status saveMsg(const EventInfo& event);

private:
   size_t size;

private:
   ABSTRACT(const void* getMsgBlock(size_t requiredSize) const);
};

///////////////////////////////////////////////////////////////////
template <cub::U16 MSG_SIZE>
struct GenericBlockBufferedEventInfo : BlockBufferedEventInfo
{
   GenericBlockBufferedEventInfo
            ( const TransStrategyDecisionMaker& strategy
            , const FailedRequestListener* listener)
      : BlockBufferedEventInfo(strategy, listener)
   {}

private:
   const void* getMsgBlock(size_t requiredSize) const
   {
      return requiredSize > sizeof(msg) ? 0 : msg;
   }

private:
   cub::U8   msg[MSG_SIZE];
};

L4_NS_END

#endif /* HD5783E6C_4701_4324_9769_CAE7C24B8376 */
