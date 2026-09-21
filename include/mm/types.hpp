#pragma once
#include <cstdint>

namespace mm {

// Prices are integer ticks, never double. Floating-point price keys in a map are a bug waiting to happen.
using Price = std::int64_t;
using Qty = std::int64_t;
using OrderId = std::uint64_t;
using Timestamp = std::int64_t;

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