#include "CalculatorExtension.h"

#include <numeric>
#include <iostream>  // debugging


/****************  PUBLIC *****************/
double Calculator::getAverage(int m) const{
    return S(m)/numMDSteps;
}


double Calculator::getAverage_slow(const std::vector<double>& x) const{
    double S = std::accumulate(x.begin(), x.end(), 0.0);
    return S/numMDSteps;
}


double Calculator::getFluctuation(int m) const{
    if(accurate){
        return getFluctuation_accurate(m);
    } else {
        return getFluctuation_inaccurate(m);
    }
}


double Calculator::getFluctuation_slow(const std::vector<double>& x) const{
    double Q = 0.;
    double av = getAverage_slow(x);
    int size = x.size();
    for(int i = 0; i < size; ++i){
        Q += (x[i]-av)*(x[i]-av);
    }
    return std::sqrt(Q/(numMDSteps-1));
}

void Calculator::computeAverageFluctuation(double x_i, int m){

    if(accurate){
        accurate_fluctuation(x_i, m);
    } else {
        inaccurate_fluctuation(x_i, m);
    }
}

// tool
void Calculator::set_mode(bool is_accurate){
    accurate = is_accurate;
}

void Calculator::indexIncrement(){
    ++index;
}



/****************  PRIVATE *****************/
// computation
void Calculator::inaccurate_fluctuation(double x_i, int m){
    S(m) += x_i;
    Ssq(m) += std::pow(x_i, 2);
}

void Calculator::accurate_fluctuation(double x_i, int m){
    // Q(m) += std::pow((S(m)-(index-1)*x_i),2)/(index*(index-1));
    Q(m) += (S(m)-(index-1)*x_i) * (S(m)-(index-1)*x_i) /(index*(index-1));

    S(m) += x_i;
    // std::cout << "step: " << index << " with Q_m = " << Q(m) << std::endl;
}


// getter
double Calculator::getFluctuation_inaccurate(int m) const{
    double var = (Ssq(m)-S(m)*S(m)/numMDSteps)/(numMDSteps-1);
    return std::sqrt(var);
}

double Calculator::getFluctuation_accurate(int m) const{
    double var = Q(m)/(numMDSteps-1);
    return std::sqrt(var);
}



