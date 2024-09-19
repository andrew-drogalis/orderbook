
#ifndef DRO_BEST_PRICE_HPP
#define DRO_BEST_PRICE_HPP

namespace dro_orderbook
{
struct BestPrice
{
  int64_t bid_size_ {};
  int64_t bid_price_ {};
  int64_t ask_price_ {};
  int64_t ask_size_ {};

  BestPrice(int64_t bid_size, int64_t bid_price, int64_t ask_price,
            int64_t ask_size)// Check param order
      : bid_size_(bid_size), bid_price_(bid_price), ask_price_(ask_price),
        ask_size_(ask_size)
  {
  }
};
}// namespace dro_orderbook
#endif
