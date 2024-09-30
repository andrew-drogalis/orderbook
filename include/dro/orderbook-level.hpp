// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_ORDERBOOK_LEVEL_HPP
#define DRO_ORDERBOOK_LEVEL_HPP

#include <cstdint>

namespace dro
{

struct OrderBookLevel
{
  int64_t price_;
  int64_t quantity_;
  uint64_t seqNum_;

  OrderBookLevel(int64_t price, int64_t quantity, uint64_t seqNum)
      : price_(price), quantity_(quantity), seqNum_(seqNum)
  {
  }
};

}// namespace dro
#endif
