#pragma once
#include <vector>
#include "mm/types.hpp"

namespace mm {

class OrderBook {
public:
    // Matches against the opposite side first; any remainder rests in the book.
    std::vector<Trade> add_limit_order(const Order& order);

    // Never rests. Unfilled remainder is dropped.
    std::vector<Trade> add_market_order(OrderId id, Side side, Qty qty, Timestamp ts);

    // Returns false if the id is unknown (already filled or never existed).
    bool cancel_order(OrderId id);

    bool has_bid() const;
    bool has_ask() const;
    Price best_bid() const;   // precondition: has_bid()
    Price best_ask() const;   // precondition: has_ask()
    Qty depth_at(Side side, Price price) const;

private:
    // YOU decide the members. See the design notes below.
};

}  // namespace mm