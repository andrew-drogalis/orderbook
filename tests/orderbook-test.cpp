// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "dro/best-bid-offer.hpp"
#include "dro/data-feed.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{

  // Best Bid Offer
  {
    uint32_t askPrice {30400};
    uint32_t askSize {500};
    uint32_t bidPrice {30100};
    uint32_t bidSize {400};
    dro::BestBidOffer bboEmpty {};
    dro::BestBidOffer bbo {bidSize, bidPrice, askSize, askPrice};
    assert(bbo.bid_size_ == bidSize);
    assert(bbo.bid_price_ == bidPrice);
    assert(bbo.ask_size_ == askSize);
    assert(bbo.ask_price_ == askPrice);
  }

  std::cout << "Test Completed!\n";
  return 0;
}
