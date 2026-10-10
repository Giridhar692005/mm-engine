#include <cstdio>
#include <cstdlib>
#include "mm/order_book.hpp"
#include "mm/market_maker.hpp"

using namespace mm;

static int g_failures = 0;

static void check_impl(bool ok, const char* expr, int line) {
    if (!ok) {
        std::printf("FAIL line %d: %s\n", line, expr);
        g_failures++;
    }
}

#define CHECK(cond) check_impl((cond), #cond, __LINE__)

void Test_market_maker(){
    Market_Maker m(0.1,2.0,1.5,1.0);
    Quotes q1 = m.compute_quotes(100,0,0);
    std::cout<<q1.bid<<" "<<q1.ask<<" "<<endl;
    
}

int main(){

     if (g_failures == 0) {
        std::printf("all tests passed\n");
        return 0;
    }
    std::printf("%d failure(s)\n", g_failures);
    return 1;
}