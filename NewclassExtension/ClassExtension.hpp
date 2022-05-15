#ifndef CLASSEXTENSION_HPP
#define CLASSEXTENSION_HPP

#include <vector>
#include <cmath>
#include <Eigen/Dense>

class Calculator{

public:

    Calculator(int numberMDSteps, int numberProperties): index(2), accurate(false)
                numMDSteps(numberMDSteps), numberProperties(numberProperties){
        S.setZero(numberProperties);
        Q.setZero(numberProperties);
        Ssq.setZero(numberProperties);
    }

    double average_slow(const std::vector<double>& x); 
    double fluctuation_slow(const std::vector<double>& x);
    
    // formula 42
    void inaccurate_fluctuation(double x_i, int m);
    
    // pair[0]: average, pair[1]:fluctuation
    void accurate_fluctuation(double x_i, int m);

    double getAverage(int m);
    double getFluctuation(int m);
    double getFluctuation_inaccurate(int m);
    double getFluctuation_accurate(int m);

    void computeAverageFluctuation(double x_i, int m);
    void set_mode(bool is_accurate){
        accurate = is_accurate;
    }

    //Correlation function
    std::vector<double> C_direct(const std::vector<double>& x);

private:

    int index;
    int numMDSteps;
    int numberProperties;
    bool accurate;
    Eigen::VectorXd S;
    Eigen::VectorXd Q;
    Eigen::VectorXd Ssq; // sum of pow(x_i,2)
        
};

#endif