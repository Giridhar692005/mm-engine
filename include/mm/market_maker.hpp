#pragma once
#include <cmath>
#include <stdexcept>
#include "mm/strategy.hpp"
namespace mm {
class Market_Maker : public Strategy{
    public:
       Market_Maker(double gamma, double sigma, double kappa, double T)
        : gamma_(gamma), sigma_(sigma), kappa_(kappa), T_(T) {
        if (gamma_ <= 0.0) throw std::invalid_argument("gamma must be > 0");
        if (sigma_ < 0.0) throw std::invalid_argument("sigma must be >= 0");
        if (kappa_ <= 0.0) throw std::invalid_argument("kappa must be > 0");
        if (T_ <= 0.0) throw std::invalid_argument("T must be > 0");
        }
        Quotes compute_quotes(double s,long q,double t) const override{
            double  tau = T_ - t;
           

            if(tau<0.0) tau=0.0;

            double var_term=gamma_*sigma_*sigma_*tau;
            
            Quotes quote;

            quote.reservation= s -static_cast<double>(q)*var_term;
            quote.spread= var_term+ (2.0/gamma_)*std::log1p(1+(gamma_/kappa_));
            
            quote.bid=quote.reservation - (quote.spread/2.0);
            quote.ask=quote.reservation + (quote.spread/2.0);
            return quote;
        }
        void set_sigma(double sigma) override{ sigma_ = sigma; }
        double sigma() const override{ return sigma_; }
    private:
    double gamma_;
    double sigma_;
    double kappa_;
    double T_;
};
}