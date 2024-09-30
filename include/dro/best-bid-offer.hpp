/// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_BEST_BID_OFFER_HPP
#define DRO_BEST_BID_OFFER_HPP

namespace dro
{

struct BestBidOffer
{
  int64_t bid_size_ {-1};
  int64_t bid_price_ {-1};
  int64_t ask_size_ {-1};
  int64_t ask_price_ {-1};

  BestBidOffer() = default;

  BestBidOffer(int64_t bid_size, int64_t bid_price, int64_t ask_size,
               int64_t ask_price)
      : bid_size_(bid_size), bid_price_(bid_price), ask_size_(ask_size),
        ask_price_(ask_price)
  {
  }
};
}// namespace dro
#endif
