// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_MESSAGE_TYPES_HPP
#define DRO_MESSAGE_TYPES_HPP

#include "dro/bit-cast.hpp"

#include <cstdint>
#include <cstring>

namespace dro
{

// Reducing memory footprint saves space is the cache and improves parsing
// performance

struct AddOrderMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OrderReferenceNumber;
  char BuySellIndicator;
  uint32_t Shares;
  uint64_t Stock;
  uint32_t Price;

  explicit AddOrderMessage(const char* buffer)
      : OrderReferenceNumber(convertToInt64(buffer + 11)),
        BuySellIndicator(buffer[19]), Shares(convertToInt32(buffer + 20)),
        Stock(convertToSymbol(buffer + 24)), Price(convertToInt32(buffer + 32))
  {
  }
};

struct AddOrderMPIDMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OrderReferenceNumber;
  char BuySellIndicator;
  uint32_t Shares;
  uint64_t Stock;
  uint32_t Price;
  // char Attribution;

  explicit AddOrderMPIDMessage(const char* buffer)
      : OrderReferenceNumber(convertToInt64(buffer + 11)),
        BuySellIndicator(buffer[19]), Shares(convertToInt32(buffer + 20)),
        Stock(convertToSymbol(buffer + 24)), Price(convertToInt32(buffer + 32))
  {
  }
};

struct OrderExecutedMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OrderReferenceNumber;
  uint32_t ExecutedShares;
  // uint64_t MatchNumber;

  explicit OrderExecutedMessage(const char* buffer)
      : OrderReferenceNumber(convertToInt64(buffer + 11)),
        ExecutedShares(convertToInt32(buffer + 19))
  {
  }
};

struct OrderExecutedWithPriceMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OrderReferenceNumber;
  uint32_t ExecutedShares;
  // uint64_t MatchNumber;
  // char Printable;
  uint32_t ExecutionPrice;

  explicit OrderExecutedWithPriceMessage(const char* buffer)
      : OrderReferenceNumber(convertToInt64(buffer + 11)),
        ExecutedShares(convertToInt32(buffer + 19)),
        ExecutionPrice(convertToInt32(buffer + 32))
  {
  }
};

struct OrderCancelMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OrderReferenceNumber;
  uint32_t CanceledShares;

  explicit OrderCancelMessage(const char* buffer)
      : OrderReferenceNumber(convertToInt64(buffer + 11)),
        CanceledShares(convertToInt32(buffer + 19))
  {
  }
};

struct OrderDeleteMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OrderReferenceNumber;

  explicit OrderDeleteMessage(const char* buffer)
      : OrderReferenceNumber(convertToInt64(buffer + 11))
  {
  }
};

struct OrderReplaceMessage
{
  // char Type;
  // uint16_t StockLocate;
  // uint16_t TrackingNumber;
  // uint64_t Timestamp;
  uint64_t OriginalOrderReferenceNumber;
  uint64_t NewOrderReferenceNumber;
  uint32_t Shares;
  uint32_t Price;

  explicit OrderReplaceMessage(const char* buffer)
      : OriginalOrderReferenceNumber(convertToInt64(buffer + 11)),
        NewOrderReferenceNumber(convertToInt64(buffer + 19)),
        Shares(convertToInt32(buffer + 27)), Price(convertToInt32(buffer + 31))
  {
  }
};

struct OrderMessage
{
  uint32_t Price {};
  uint32_t Shares {};
  uint16_t LimitOrderBookID {};
  char BuySellIndicator {};

  OrderMessage() = default;
  explicit OrderMessage(const AddOrderMessage& message, uint16_t orderbookID)
      : Price(message.Price), Shares(message.Shares),
        LimitOrderBookID(orderbookID),
        BuySellIndicator(message.BuySellIndicator)
  {
  }
  explicit OrderMessage(const AddOrderMPIDMessage& message,
                        uint16_t orderbookID)
      : Price(message.Price), Shares(message.Shares),
        LimitOrderBookID(orderbookID),
        BuySellIndicator(message.BuySellIndicator)
  {
  }
  OrderMessage(uint32_t price, uint32_t shares, char buySell,
               uint16_t orderbookID)
      : Price(price), Shares(shares), BuySellIndicator(buySell),
        LimitOrderBookID(orderbookID)
  {
  }
};

}// namespace dro
#endif
