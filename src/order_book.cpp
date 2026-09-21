#include "mm/order_book.hpp"

namespace mm {

std::vector<Trade> OrderBook::add_limit_order(const Order&) { return {}; }
std::vector<Trade> OrderBook::add_market_order(OrderId, Side, Qty, Timestamp) { return {}; }
bool OrderBook::cancel_order(OrderId) { return false; }
bool OrderBook::has_bid() const { return false; }
bool OrderBook::has_ask() const { return false; }
Price OrderBook::best_bid() const { return 0; }
Price OrderBook::best_ask() const { return 0; }
Qty OrderBook::depth_at(Side, Price) const { return 0; }

}  // namespace mm