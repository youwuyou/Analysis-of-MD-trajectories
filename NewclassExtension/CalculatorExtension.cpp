#include "CalculatorExtension.h"

#include <numeric>
#include <iostream>  // debugging


/****************  PUBLIC *****************/
double Calculator::getAverage(int m) const{
    return S(m)/numMDSteps;
}


double Calculator::getFluctuation(int m) const{
    if(accurate){
        return getFluctuation_accurate(m);
    } else {
        return getFluctuation_inaccurate(m);
    }
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

void Calculator::set_duration(double time){
    duration_ns += time;
}

 void Calculator::print_timing(std::ostream &out) const{
     out << " Time spent for average and fluctuation calculation: " << std::setprecision(3) << duration_ns << " nanosec" << std::endl;
 }



/****************  PRIVATE *****************/
// computation
void Calculator::inaccurate_fluctuation(double x_i, int m){
    S(m) += x_i;
    Ssq(m) += std::pow(x_i, 2);
}

void Calculator::accurate_fluctuation(double x_i, int m){

    Q(m) += (S(m)-(index-1)*x_i) * (S(m)-(index-1)*x_i) /(index*(index-1));

    S(m) += x_i;
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



