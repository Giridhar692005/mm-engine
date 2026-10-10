#pragma once

struct Quotes{
    double bid;
    double ask;
    double reservation;
    double spread;
}

class Strategy{
    public:
        virtual ~Strategy(){}
        virtual Quotes compute_quotes(double s,long q,double t) const=0;
        virtual void set_sigma(double sigma) =0;
        virtual double sigma() const = 0;
};