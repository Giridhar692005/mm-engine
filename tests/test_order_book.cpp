#include <cstdio>
#include <cstdlib>
#include "mm/order_book.hpp"

using namespace mm;

static int g_failures = 0;

static void check_impl(bool ok, const char* expr, int line) {
    if (!ok) {
        std::printf("FAIL line %d: %s\n", line, expr);
        g_failures++;
    }
}

#define CHECK(cond) check_impl((cond), #cond, __LINE__)

void test_rest_without_cross() {
    OrderBook book;
    std::vector<Trade> trades = book.add_limit_order(Order{1, Side::Buy, 100, 10, 1});
    CHECK(trades.empty());
    CHECK(book.has_bid());
    CHECK(!book.has_ask());
    CHECK(book.best_bid() == 100);
    CHECK(book.depth_at(Side::Buy, 100) == 10);
}

// TODO(you): fill these in later
void test_full_fill() {
    OrderBook book;

book.add_limit_order(Order{1, Side::Sell, 100, 10, 1});

std::vector<Trade> trades=book.add_limit_order(Order{2, Side::Buy, 100, 10, 2});
    CHECK(trades.size()==1);
    CHECK(trades[0].qty == 10);
    CHECK(trades[0].price == 100);
    CHECK(trades[0].taker_id == 2);
    CHECK(trades[0].maker_id == 1);
    CHECK(!book.has_bid());
    CHECK(!book.has_ask());
}
void test_partial_fill_then_rest() {
    OrderBook book;
    book.add_limit_order(Order{1, Side::Sell, 100, 10, 1});
    std::vector<Trade> trades=book.add_limit_order(Order{2, Side::Buy, 100, 15, 2});
    CHECK(trades.size()==1);
    CHECK(trades[0].qty == 10);
    CHECK(trades[0].price == 100);
    CHECK(trades[0].taker_id == 2);
    CHECK(trades[0].maker_id == 1);
    CHECK(book.depth_at(Side::Buy, 100) == 5);
    CHECK(!book.has_ask());
}

void test_price_priority() {}
void test_time_priority_same_price() {}
void test_trade_price_is_maker_price() {}
void test_market_order_sweeps_levels() {}
void test_market_order_on_empty_book() {}
void test_cancel_removes_liquidity() {}
void test_cancel_unknown_id() {}
void test_cancel_after_partial_fill() {}

int main() {
    test_rest_without_cross();
    test_full_fill();
    test_partial_fill_then_rest();
    test_price_priority();
    test_time_priority_same_price();
    test_trade_price_is_maker_price();
    test_market_order_sweeps_levels();
    test_market_order_on_empty_book();
    test_cancel_removes_liquidity();
    test_cancel_unknown_id();
    test_cancel_after_partial_fill();

    if (g_failures == 0) {
        std::printf("all tests passed\n");
        return 0;
    }
    std::printf("%d failure(s)\n", g_failures);
    return 1;
}