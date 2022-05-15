#include "ClassExtension.hpp"
#include <numeric>

double Calculator::average_slow(const std::vector<double>& x){
    double S = std::accumulate(x.begin(), x.end(), 0.0);
    return S/numMDSteps;
}

double Calculator::fluctuation_slow(const std::vector<double>& x){
    double Q = 0.;
    double av = average_slow(x);
    int size = x.size();
    for(int i = 0; i < size; ++i){
        Q += (x[i]-av)*(x[i]-av);
    }
    return std::sqrt(Q/(numMDSteps-1));
}

void Calculator::inaccurate_fluctuation(double x_i, int m){
    S(m) += x_i;
    Ssq(m) += std::pow(x_i, 2);
}

void Calculator::accurate_fluctuation(double x_i, int m){
    Q(m) += std::pow((S-(index-1)*x_i),2)/(index*(index-1));
    S(m) += x_i;
    ++index;
}

double Calculator::getAverage(int m){
    return S(m)/numMDSteps;
}

double Calculator::getFluctuation(int m){
    if(accurate){
        return getFluctuation_accurate(m);
    } else {
        return getFluctuation_inaccurate(m);
    }
}

double Calculator::getFluctuation_inaccurate(int m){
    double var = (Ssq(m)-S(m)*S(m)/numMDSteps)/(numMDSteps-1);
    return std::sqrt(var); 
}

double Calculator::getFluctuation_accurate(int m){
    double var = Q(m)/(numMDSteps-1);
    return std::sqrt(var); 
}

void Calculator::computeAverageFluctuation(double x_i, int m){
    if(accurate){
        accurate_fluctuation(x_i, m);
    } else {
        inaccurate_fluctuation(x_i, m);
    }
}

std::vector<double> Calculator::C_direct(const std::vector<double>& x){
    std::vector<double> ret(numMDSteps);

    for(int n = 0; n < numMDSteps; ++n){
        double S = 0.;
        for(int k = 0; k < numMDSteps - n; ++k){
            S += x[k]*x[k+n];
        }
        S /= (numMDSteps-n);
        ret[n] = S;
    }
}