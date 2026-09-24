#include <random>
#include "mm/order_book.hpp"
#include "mm/types.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace mm;
std::mt19937 rng(std::random_device{}());          
Order generate_random_order(OrderId id, Price center_price, Timestamp ts){
    std::uniform_int_distribution<int> sideprob(0, 1);
    Side s=(sideprob(rng))? Side::Buy : Side::Sell;
    std::uniform_int_distribution<int> dist(-10, 10);
    Price price=center_price+dist(rng);
    std::uniform_int_distribution<Qty> qtyprob(1, 20);
    Qty qt=qtyprob(rng);
    
    return Order{id,s,price,qt,ts};
    
} 
void run_simulation(int num_events,Price center_price){
    OrderBook book;
    for(int i=1;i<=num_events;i++){
        Order order=generate_random_order(i,center_price,i);
        std::string s=(order.side == Side::Buy ? "BUY" : "SELL");
        std::cout <<"order: \nId ="<<order.id<<"\nSide="<<s<< "\nprice=" << order.price << "\nqty=" << order.qty << "\n";
        std::vector<Trade> trades=book.add_limit_order(order);
        if(!trades.empty()){
            for(auto trade:trades){
                std::cout<<"Loop NO:"<<i<<"\n";
                std::cout<<"Trade:\t"<<trade.taker_id<<"\t"<<trade.maker_id<<"\t"<<trade.price<<"\t"<<trade.qty<<"\t"<<trade.ts<<"\n";
            }
        }

    }
}                         
int main() {
    run_simulation(20, 100);
    return 0;
}