// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_ITCH_PARSER_HPP
#define DRO_ITCH_PARSER_HPP

#include "include/dro/data-feed.hpp"
#include "include/dro/message-types.hpp"

#include <cstdint>
#include <cstring>

namespace dro
{

class ItchParser
{
public:
  explicit ItchParser(DataFeed& feed) : feed_(feed) {}

  void Parse(const char* buffer)
  {
    switch (buffer[0])
    {
    case 'A':
      AddOrder(buffer);
    case 'F':
      AddOrderMPID(buffer);
    case 'E':
      OrderExecuted(buffer);
    case 'C':
      OrderExecutedWithPrice(buffer);
    case 'X':
      OrderCancel(buffer);
    case 'D':
      OrderDelete(buffer);
    case 'U':
      OrderReplace(buffer);
    case 'S':
      // SystemEvent
    case 'R':
      // StockDirectory
    case 'H':
      // StockTradingAction
    case 'Y':
      // RegSHO
    case 'L':
      // MarketParticipantPosition
    case 'V':
      // MWCBDecline
    case 'W':
      // MWCBStatus
    case 'K':
      // IPOQuoting
    case 'P':
      // Trade
    case 'Q':
      // CrossTrade
    case 'B':
      // BrokenTrade
    case 'I':
      // NOII
    case 'N':
      // RPII
    case 'J':
      // LULDAuctionCollar
    default:
    }
  }

private:
  void AddOrder(const char* buffer)
  {
    feed_.AddOrder(AddOrderMessage {buffer});
  }

  void AddOrderMPID(const char* buffer)
  {
    feed_.AddOrder(AddOrderMPIDMessage {buffer});
  }

  void OrderExecuted(const char* buffer)
  {
    feed_.OrderExecuted(OrderExecutedMessage {buffer});
  }

  void OrderExecutedWithPrice(const char* buffer)
  {
    feed_.OrderExecutedWithPrice(OrderExecutedWithPriceMessage {buffer});
  }

  void OrderCancel(const char* buffer)
  {
    feed_.ReduceOrder(OrderCancelMessage {buffer});
  }

  void OrderDelete(const char* buffer)
  {
    feed_.DeleteOrder(OrderDeleteMessage {buffer});
  }

  void OrderReplace(const char* buffer)
  {
    feed_.ModifyOrder(OrderReplaceMessage {buffer});
  }

  DataFeed& feed_;
};

}// namespace dro

#endif
