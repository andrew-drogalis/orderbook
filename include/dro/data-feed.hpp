// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_DATA_FEED_HPP
#define DRO_DATA_FEED_HPP

#include "dro/bit-cast.hpp"
#include "dro/limit-orderbook.hpp"
#include "dro/message-types.hpp"
#include "Open-Addressing-Hashmap/include/dro/oa-hashmap.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

namespace dro
{

class DataFeed
{
private:
  bool allStocks_ {};
  uint64_t ordersProcessed_ {};
  std::vector<LimitOrderBook> limitOrderBooks_;
  HashMap<uint64_t, uint32_t> symbolsMap_;
  HashMap<uint64_t, OrderMessage> ordersMap_;

public:
  DataFeed(size_t initialOrderSize, size_t initialSymbolSize)
      : symbolsMap_(initialSymbolSize, 0),
        ordersMap_(initialOrderSize, std::numeric_limits<uint64_t>::max())
  {
  }

  ~DataFeed() = default;
  // Not copyable and Not moveable
  DataFeed(const DataFeed&)            = delete;
  DataFeed& operator=(const DataFeed&) = delete;
  DataFeed(DataFeed&&)                 = delete;
  DataFeed& operator=(DataFeed&&)      = delete;

  void Parse(const char* buffer)
  {
    switch (buffer[0])
    {
    case 'A':
      AddOrder(buffer);
    case 'F':
      AddOrder(buffer);
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
      return;
    }
  }

  void setAllStocks(bool allStocks) { allStocks_ = allStocks; }

  [[nodiscard]] bool getAllStocks() const { return allStocks_; }

  void Subscribe(std::string stock)
  {
    if (stock.size() < 8)
    {
      stock.insert(stock.size(), 8 - stock.size(), ' ');
    }
    uint64_t symbol = convertToSymbol(stock.data());
    auto it         = symbolsMap_.find(symbol);
    if (it == symbolsMap_.end())
    {
      limitOrderBooks_.emplace_back();
      it = symbolsMap_.emplace(symbol, limitOrderBooks_.size() - 1).first;
    }
  }

  void AddOrder(const char* buffer)
  {
    AddOrderMessage message {buffer};
    auto it = symbolsMap_.find(message.Stock);

    if (it == symbolsMap_.end())
    {
      if (allStocks_)
      {
        limitOrderBooks_.emplace_back();
        it = symbolsMap_.emplace(message.Stock, limitOrderBooks_.size() - 1)
                 .first;
      }
      else
      {
        return;
      }
    }
    int32_t bookid       = it->second;
    LimitOrderBook& book = limitOrderBooks_[bookid];
    if (ordersMap_
            .emplace(message.OrderReferenceNumber,
                     OrderMessage(message, bookid))
            .second)
    {
      book.AddToBook(message.BuySellIndicator, message.Price, message.Shares);
    }
    ++ordersProcessed_;
  }

  void OrderCancel(const char* buffer)
  {
    OrderCancelMessage message {buffer};
    auto it = ordersMap_.find(message.OrderReferenceNumber);
    if (it == ordersMap_.end())
    {
      return;
    }

    OrderMessage& order  = it->second;
    LimitOrderBook& book = limitOrderBooks_[order.LimitOrderBookID];
    book.ReduceFromBook(order.BuySellIndicator, order.Price,
                        message.CanceledShares);
    order.Shares -= message.CanceledShares;
    if (order.Shares <= 0)
    {
      ordersMap_.erase(it);
    }
    ++ordersProcessed_;
  }

  void OrderDelete(const char* buffer)
  {
    OrderDeleteMessage message {buffer};
    auto it = ordersMap_.find(message.OrderReferenceNumber);
    if (it == ordersMap_.end())
    {
      return;
    }

    OrderMessage& order  = it->second;
    LimitOrderBook& book = limitOrderBooks_[order.LimitOrderBookID];
    book.ReduceFromBook(order.BuySellIndicator, order.Price, order.Shares);
    ordersMap_.erase(it);
    ++ordersProcessed_;
  }

  void OrderReplace(const char* buffer)
  {
    OrderReplaceMessage message {buffer};
    auto it = ordersMap_.find(message.OriginalOrderReferenceNumber);
    if (it == ordersMap_.end())
    {
      return;
    }

    OrderMessage& order  = it->second;
    LimitOrderBook& book = limitOrderBooks_[order.LimitOrderBookID];
    book.ReduceFromBook(order.BuySellIndicator, order.Price, order.Shares);
    book.AddToBook(order.BuySellIndicator, message.Price, message.Shares);

    ordersMap_.erase(it);
    ordersMap_.emplace(message.NewOrderReferenceNumber,
                       OrderMessage(message.Price, message.Shares,
                                    order.BuySellIndicator,
                                    order.LimitOrderBookID));
    ++ordersProcessed_;
  }

  void OrderExecuted(const char* buffer)
  {
    OrderExecutedMessage message {buffer};
    auto it = ordersMap_.find(message.OrderReferenceNumber);
    if (it == ordersMap_.end())
    {
      return;
    }

    OrderMessage& order  = it->second;
    LimitOrderBook& book = limitOrderBooks_[order.LimitOrderBookID];
    book.ReduceFromBook(order.BuySellIndicator, order.Price,
                        message.ExecutedShares);

    order.Shares -= message.ExecutedShares;
    if (order.Shares <= 0)
    {
      ordersMap_.erase(it);
    }
    ++ordersProcessed_;
  }

  void OrderExecutedWithPrice(const char* buffer)
  {
    OrderExecutedWithPriceMessage message {buffer};
    auto it = ordersMap_.find(message.OrderReferenceNumber);
    if (it == ordersMap_.end())
    {
      return;
    }

    OrderMessage& order = it->second;
    if (message.ExecutionPrice != order.Price)
    {
      return;
    }
    LimitOrderBook& book = limitOrderBooks_[order.LimitOrderBookID];
    book.ReduceFromBook(order.BuySellIndicator, order.Price,
                        message.ExecutedShares);

    order.Shares -= message.ExecutedShares;
    if (order.Shares <= 0)
    {
      ordersMap_.erase(it);
    }
    ++ordersProcessed_;
  }

  [[nodiscard]] size_t symbolCount() const { return symbolsMap_.size(); }

  [[nodiscard]] size_t ordersCount() const { return ordersProcessed_; }
};

}// namespace dro
#endif
