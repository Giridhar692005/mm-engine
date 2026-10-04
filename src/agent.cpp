#include "mm/agent.hpp"
#include "mm/market_maker.hpp"
#include "rolling_volatility.hpp"
#include <cmath>

Agent::Agent(Strategy* strategy,OrderBook* book, double tick, int quote_qty,long long id_base, long max_inventory):
       strategy_(strategy),book_(book),vol_(NULL),tick_(tick),quote_qty_(quote_qty),
       id_base_(id_base),max_inventory_(max_inventory),
       live_bid_id_(0), live_ask_id_(0), inventory_(0), cash_(0.0),
       last_mid_(0.0), has_mid_(false), last_bid_(0.0), last_ask_(0.0),
       last_reservation_(0.0) {}

void Agent::on_trades(const std::vector<trade>& trades){
      for(size_t i=0;i < trades.size(); ++i){
        const Trade& t=trades[i];
        if( live_bid_id_ !=0 && t.maker_id==live_bid_id_){
            inventory_ += t.qty;
            cash_ -= t.price*static_cast<double>(t.qty);
        }else
        if(live_ask_id_!=0 && t.maker_id==live_ask_id_){
            inventory_ -= t.qty;
            cash_ += t.price*static_cast<double>(t.qty);
        }
      }
}

void Agent::cancel_live_quotes(){

   if(live_bid_id_ != 0){book_->cancel_order(live_bid_id_); live_bid_id_=0;}
   if(live_ask_id_ != 0){book_->cancel_order(live_ask_id_); live_ask_id_=0;}
}

bool Agent::requote(double ts, double t_norm) {
    cancel_live_quotes();

    double m;
    if (book_.get_mid_price(m)) { last_mid_ = m; has_mid_ = true; }
    if (!has_mid_) return false;

    Quotes qt = strategy_->compute_quotes(last_mid_, inventory_, t_norm);
    last_reservation_ = qt.reservation;

    long long bid_ticks = static_cast<long long>(std::floor(qt.bid / tick_ + 1e-9));
    long long ask_ticks = static_cast<long long>(std::ceil (qt.ask / tick_ - 1e-9));
    if (ask_ticks <= bid_ticks) ask_ticks = bid_ticks + 1;

    double bid = static_cast<double>(bid_ticks) * tick_;
    double ask = static_cast<double>(ask_ticks) * tick_;

    if (book_.has_ask() && bid >= book_.best_ask()) bid = book_.best_ask() - tick_;
    if (book_.has_bid() && ask <= book_.best_bid()) ask = book_.best_bid() + tick_;

    bid = static_cast<double>(std::llround(bid / tick_)) * tick_;
    ask = static_cast<double>(std::llround(ask / tick_)) * tick_;

    if (bid >= ask) return false;

    bool can_bid = (inventory_ < max_inventory_);
    bool can_ask = (inventory_ > -max_inventory_);

    if (can_bid) {
        Order o;
        o.id = next_id_++;
        o.side = Side::Buy;
        o.price = bid;
        o.qty = 1LL;
        o.ts = ts;
        live_bid_id_ = o.id;                      
        last_bid_ = bid;
        on_trades(book_.add_limit_order(o));
    }

    if (can_ask) {
        Order o;
        o.id = next_id_++;
        o.side = Side::Sell;
        o.price = ask;
        o.qty = 1LL;
        o.ts = ts;
        live_ask_id_ = o.id;                       
        last_ask_ = ask;
        on_trades(book_.add_limit_order(o));
    }

    return true;
}
