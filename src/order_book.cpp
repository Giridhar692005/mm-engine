#include "mm/order_book.hpp"
#include "mm/types.hpp"
#include <algorithm>
namespace mm {

std::vector<Trade> OrderBook::add_limit_order(const Order& order) { 
    std::vector<Trade> trade_;
    if(order.side==Side::Buy){
        Qty remaining = order.qty;
        while(remaining>0&&!asks_.empty()&&order.price>=best_ask()){
           Order& maker = asks_.begin()->second.front();
           Qty fill = std::min(remaining, maker.qty);
           trade_.push_back(Trade{order.id, maker.id, maker.price, fill, maker.ts});
           remaining -= fill;
           maker.qty -= fill;
           if(maker.qty==0){
               asks_.begin()->second.pop_front();
               if(asks_.begin()->second.empty()){
                    asks_.erase(asks_.begin());
               }
           }
        }
        if(remaining>0){
            bids_[order.price].push_back(Order{order.id, order.side, order.price, remaining, order.ts});
        }
    }
    else
    if(order.side==Side::Sell){
        
        Qty remaining = order.qty;
        while(remaining>0&&!bids_.empty()&&order.price<=best_bid()){
           Order& maker = bids_.begin()->second.front();
           Qty fill = std::min(remaining, maker.qty);
           trade_.push_back(Trade{order.id, maker.id, maker.price, fill, maker.ts});
           remaining -= fill;
           maker.qty -= fill;
           if(maker.qty==0){
               bids_.begin()->second.pop_front();
               if(bids_.begin()->second.empty()){
                    bids_.erase(bids_.begin());
               }
           }
        }
        if(remaining>0){
            asks_[order.price].push_back(Order{order.id, order.side, order.price, remaining, order.ts});
        }
    }
    
    return trade_; 
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
Price OrderBook::best_ask() const { 
    return asks_.begin()->first; 
}
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

} 