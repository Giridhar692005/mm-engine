#pragma once
#include <vector>
#include <deque>
#include <utility>
#include <cmath>

namespace mm{
inline double realized_sigma(const std::vector<double>& ts, const std::vector<double>& mid){
    double delta_t = 0 , delta_mid = 0;

    for(size_t i=1; i < ts.size();i++){
        if(ts[i]-ts[i-1] > 0){
            delta_t += (ts[i]-ts[i-1]);
            double d = mid[i]-mid[i-1];
            delta_mid += d*d;
        }
    }
    if(delta_t == 0 || delta_mid == 0) return 0;
    double sigma = std::sqrt(delta_mid/delta_t);
    return sigma;
}

class RollingVolatility {
public:
    explicit RollingVolatility(size_t window): window_(window) {
        last_mid_ = 0;
        last_ts_ = 0;
        sum_sq_ = 0;
        sum_dt_= 0;
        is_ready = false;
        sigma_=0;
        has_last_= false;
    }
    void   update(double ts, double mid){
        if(has_last_ == false){
            last_mid_ = mid;
            last_ts_ = ts;
            has_last_ = true;
        }
        else{
           double dt = ts - last_ts_;
           if( dt <= 0 )  return ;
           double d2 = mid - last_mid_;
           q.push_back(std::make_pair(dt , d2));
           if(q.size() == window_) is_ready = true;
           if(q.size() > window_){
                double k = q.front().second;
                sum_sq_ -= k*k ;
                sum_dt_ -= q.front().first;
                q.pop_front();
           }  
           sum_sq_ += d2*d2;
           sum_dt_ += dt;
           last_mid_ = mid;
           last_ts_ = ts;
        }
         
    }
    bool   ready() const{ return is_ready;}           
    double sigma() const{
        if(sum_dt_ <= 0|| sum_sq_ <= 0) return 0; 
         
        return std::sqrt(sum_sq_/sum_dt_);
    }                  
private:
    size_t window_;
    double last_mid_ ;
    double last_ts_ ;
    double sum_sq_;
    double sum_dt_;
    bool has_last_;
    bool is_ready;
    std::deque<std::pair<double,double>> q;

};
}