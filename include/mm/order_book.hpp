#pragma once
#include <functional>
#include <list>
#include <map>
#include <vector>
#include "mm/types.hpp"

namespace mm {

class OrderBook {
public:
    std::vector<Trade> add_limit_order(const Order& order);
    std::vector<Trade> add_market_order(OrderId id, Side side, Qty qty, Timestamp ts);
    bool cancel_order(OrderId id);

    bool has_bid() const;
    bool has_ask() const;
    Price best_bid() const;
    Price best_ask() const;
    Qty depth_at(Side side, Price price) const;

private:
    std::map<Price, std::list<Order>, std::greater<Price>> bids_;
    std::map<Price, std::list<Order>> asks_;
};

} 