#include "ClassExtension.hpp"
#include <numeric>

double Calculator::average_slow(const std::vector<double>& x){
    double S = std::accumulate(x.begin(), x.end(), 0.0);
    return S/numMDSteps;
}

double Calculator::fluctuation_slow(const std::vector<double>& x){
    double Q = 0.;
    double av = average_slow(x);
    for(int i = 0; i < numMDSteps; ++i){
        Q += (x[i]-av)*(x[i]-av);
    }
    return Q/(numMDSteps-1);
}

double Calculator::inaccurate_fluctuation(const std::vector<double>& x){
    double Ssq = 0; // sum of x squared
    double S = 0; // sum of x
    for(int i = 0; i < numMDSteps; ++i){
        S += x[i];
        Ssq += x[i]*x[i];
    }
    return (Ssq - S*S/numMDSteps)/(numMDSteps -1);
}

std::pair<double, double> Calculator::AverageFluctuation_fast(const std::vector<double>& x){
    double S = 0.;
    double Q = 0.;
    for(int i = 0; i < numMDSteps; ++i){
       int I = i+1;
       Q += std::pow((S-(I-1)*x[i]), 2)/(I*(I-1));
       S += x[i];
    }
    auto ret = std::make_pair(S/numMDSteps, Q/(numMDSteps-1));

    return ret;
}

std::vector<double> Calculator::C_direct(const std::vector<double>& x){
    std::vector<double> ret;

    for(int n = 0; n < numMDSteps; ++n){
        double S = 0.;
        for(int k = 0; k < numMDSteps - n; ++k){
            S += x[k]*x[k+n];
        }
        S /= (numMDSteps-n);
        ret[n] = S;
    }
}