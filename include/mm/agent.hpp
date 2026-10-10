#pragma once
#include <vector>
#include "order_book.hpp"
#include "strategy.hpp"

class Agent{
    public:
        Agent(Strategy* strategy,OrderBook* book, double tick, int quote_qty,long long id_base, long max_inventory);
        void set_volatility_estimator(RollingVolatility* vol) { vol_ = vol; }

        void on_trades(const std::vector<Trade>& trades);
        bool requote(double ts, double t_norm);

        long   inventory() const { return inventory_; }
        double cash() const { return cash_; }
        double last_mid() const { return last_mid_; }
        double wealth() const { return cash_ + static_cast<double>(inventory_) * last_mid_; }
        double last_bid() const { return last_bid_; }
        double last_ask() const { return last_ask_; }
        double last_reservation() const { return last_reservation_; }
        bool   is_mine(long long id) const { return id >= id_base_; }
        void cancel_live_quotes();
    private:
       
       Strategy* strategy_;
       OrderBook* book_;
       RollingVolatility* vol_;
       double tick_;
       int quote_qty_;
       long long id_base_;
       long long next_id_;
       long max_inventory_;
       long long live_bid_id_;     // 0 means none
       long long live_ask_id_;
       long inventory_;
       double cash_;
       double last_mid_;
       bool has_mid_;
       double last_bid_;
       double last_ask_;
       double last_reservation_;

    
};