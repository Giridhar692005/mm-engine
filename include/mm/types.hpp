#pragma once
#include <cstdint>

namespace mm {

// Prices are integer ticks, never double. Floating-point price keys in a map are a bug waiting to happen.
using Price = double;
using Qty = std::long long;
using OrderId = std::long long;
using Timestamp = double;

enum class Side { Buy, Sell };

struct Order {
    OrderId id;
    Side side;
    Price price;
    Qty qty;
    Timestamp ts;
};

struct Trade {
    OrderId taker_id;
    OrderId maker_id;
    Price price;  
    Qty qty;
    Timestamp ts;
};
}