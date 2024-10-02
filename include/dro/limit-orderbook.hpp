// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_ORDERBOOK_HPP
#define DRO_ORDERBOOK_HPP

#include <cstdint>

#include "Flat-Map-RB-Tree/include/dro/flat-rb-tree.hpp"
#include "dro/best-bid-offer.hpp"

namespace dro
{

struct OrderBookLevel
{
  uint32_t quantity_ {};
  // In future can do MBO rather then MBP
};

class LimitOrderBook
{
public:
  using price_level = uint32_t;
  using bid_level_map =
      FlatMap<price_level, OrderBookLevel, uint16_t, std::greater<price_level>>;
  using offer_level_map =
      FlatMap<price_level, OrderBookLevel, uint16_t, std::less<price_level>>;
  using best_bid_offer = BestBidOffer;

private:
  static constexpr std::size_t initialLevelSize {100};
  bid_level_map bidLevels_ {initialLevelSize};
  offer_level_map offerLevels_ {initialLevelSize};

public:
  LimitOrderBook() = default;

  [[nodiscard]] best_bid_offer GetBestBidOffer() const
  {
    auto minOffer = offerLevels_.begin();
    auto maxBid   = bidLevels_.begin();
    if (minOffer == offerLevels_.end() || maxBid == bidLevels_.end())
    {
      return best_bid_offer {};
    }
    return {maxBid->second.quantity_, maxBid->first, minOffer->second.quantity_,
            minOffer->first};
  }

  void AddToBook(char buy_sell, uint32_t price, uint32_t quantity)
  {
    if (buy_sell == 'B')
    {
      auto it = bidLevels_.emplace(price).first;
      it->second.quantity_ += (quantity > 0) ? quantity : 0;
      return;
    }
    auto it = offerLevels_.emplace(price).first;
    it->second.quantity_ += (quantity > 0) ? quantity : 0;
  }

  void ReduceFromBook(char buy_sell, uint32_t price, uint32_t quantity)
  {
    if (buy_sell == 'B')
    {
      auto it = bidLevels_.find(price);
      if (it == bidLevels_.end())
      {
        return;
      }
      it->second.quantity_ -= quantity;
      if (it->second.quantity_ <= 0)
      {
        bidLevels_.erase(it);
      }
    }
    auto it = offerLevels_.find(price);
    if (it == offerLevels_.end())
    {
      return;
    }
    it->second.quantity_ -= quantity;
    if (it->second.quantity_ <= 0)
    {
      offerLevels_.erase(it);
    }
  }

  bool IsCrossed()
  {
    if (offerLevels_.empty() || bidLevels_.empty())
    {
      return false;
    }
    return offerLevels_.begin()->first <= bidLevels_.begin()->first;
  }
};
}// namespace dro
#endif
