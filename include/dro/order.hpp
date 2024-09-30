// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_ORDER_HPP
#define DRO_ORDER_HPP

#include <cstdint>

namespace dro
{

struct Order
{
  int64_t price_;
  int32_t quantity_;
  int16_t orderbookID_;
  bool buySell_;

  Order(int64_t price, int32_t quantity, int16_t buySell, int16_t orderbookID)
      : price_(price), quantity_(quantity), buySell_(buySell),
        orderbookID_(orderbookID)
  {
  }
};

}// namespace dro

#endif
