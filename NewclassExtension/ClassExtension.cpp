#include "ClassExtension.hpp"
#include <numeric>

double Calculator::average_slow(){
    double S = std::accumulate(x_.begin(), x_.end(), 0.0);
    return S/N_x;
}

double Calculator::fluctuation_slow(){
    double Q = 0.;
    double av = average_slow();
    for(int i = 0; i < N_x; ++i){
        Q += (x_[i]-av)*(x_[i]-av);
    }
    return Q/(N_x-1);
}

std::pair<double, double> Calculator::AverageFluctuation_fast(){
    double S = 0.;
    double Q = 0.;
    for(int i = 0; i < N_x; ++i){
       int I = i+1;
       Q += std::pow((S-(I-1)*x_[i]), 2)/(I*(I-1));
       S += x_[i];
    }
    auto ret = std::make_pair(S/N_x, Q/(N_x-1));

    return ret;
}