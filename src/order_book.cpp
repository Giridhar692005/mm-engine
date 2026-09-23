#include "mm/order_book.hpp"
#include "mm/types.hpp"
namespace mm {

std::vector<Trade> OrderBook::add_limit_order(const Order& order) { 
    if(order.side==Side::Buy){
        bids_[order.price].push_back(order);
    }
    else
    if(order.side==Side::Sell){
        asks_[order.price].push_back(order);
    }
    return {}; 
}
std::vector<Trade> OrderBook::add_market_order(OrderId, Side, Qty, Timestamp) { return {}; }
bool OrderBook::cancel_order(OrderId) { return false; }
bool OrderBook::has_bid() const {
    return !bids_.empty();
}
bool OrderBook::has_ask() const { 
    return !asks_.empty(); }
Price OrderBook::best_bid() const { 
    return bids_.begin()->first; 
}
Price OrderBook::best_ask() const { return 0; }
Qty OrderBook::depth_at(Side side, Price price) const { 
    Qty qt=0;
    if(side==Side::Buy){
      auto it =bids_.find(price);
      if(it!=bids_.end()){
        for(const Order& o: it->second){
         qt+=o.qty;
        }
       }
    }else
    if(side ==Side::Sell){
      auto it =asks_.find(price);
      if(it!=asks_.end()){
        for(const Order& o: it->second){
         qt+=o.qty;
        }
    }
    }
    return qt; 
}

}  // namespace mm