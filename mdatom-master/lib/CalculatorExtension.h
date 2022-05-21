#ifndef CALCULATOREXTENSION_H
#define CALCULATOREXTENSION_H

#include <vector>
#include <cmath>

#include <Eigen/Core> // using Eigen


/*********** TASK 1 ***************/

class Calculator{


public:

    Calculator(int numberMDSteps, int numberProperties): index(2), numMDSteps(numberMDSteps),
                 numberProperties(numberProperties), accurate(false)
    {
        S.setZero(numberProperties);
        Q.setZero(numberProperties);
        Ssq.setZero(numberProperties);
    }

    // Getters - given index of properties obtain corres. property entry
    double getAverage(int m) const;
    double getAverage_slow(const std::vector<double>& x) const;

    double getFluctuation(int m) const;
    double getFluctuation_slow(const std::vector<double>& x) const;


    // computation wrapper for each step
    void computeAverageFluctuation(double x_i, int m);

    // helpers
    void set_mode(bool is_accurate);
    void indexIncrement();




private:
    /******* private parameters ********/
    int index;
    int numMDSteps;
    int numberProperties;
    bool accurate;
    Eigen::VectorXd S;
    Eigen::VectorXd Q;
    Eigen::VectorXd Ssq; // sum of pow(x_i,2)


    /******* private methods ********/
    void inaccurate_fluctuation(double x_i, int m); // formula 42
    void accurate_fluctuation(double x_i, int m);   // formula 43, 45 - pair[0]: average, pair[1]:fluctuation

    // internal getters - used for getFluctuation
    double getFluctuation_inaccurate(int m) const;
    double getFluctuation_accurate(int m) const;

};

#endif