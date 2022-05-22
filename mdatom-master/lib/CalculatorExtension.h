#ifndef CALCULATOREXTENSION_H
#define CALCULATOREXTENSION_H

#include <vector>
#include <cmath>
#include <iomanip>

#include <Eigen/Core> // using Eigen


/*********** TASK 1 ***************/

class Calculator{


public:

    Calculator(int numberMDSteps, int numberProperties): index(2), numMDSteps(numberMDSteps),
                 /*numberProperties(numberProperties),*/ accurate(false), duration_ns(0.)
    {
        S.setZero(numberProperties);
        Q.setZero(numberProperties);
        Ssq.setZero(numberProperties);
    }

    // Getters - given index of properties obtain corres. property entry
    double getAverage(int m) const;

    double getFluctuation(int m) const;

    // computation wrapper for each step
    void computeAverageFluctuation(double x_i, int m);

    // helpers
    void set_mode(bool is_accurate);
    void indexIncrement();

    //timing
    void set_duration(double time);
    void print_timing(std::ostream &out) const;




private:
    /******* private parameters ********/
    int index;
    int numMDSteps;
    //int numberProperties;
    bool accurate;
    Eigen::VectorXd S;
    Eigen::VectorXd Q;
    Eigen::VectorXd Ssq; // sum of pow(x_i,2)
    // std::chrono::system_clock::time_point tStart1, tEnd1;
    double duration_ns;


    /******* private methods ********/
    void inaccurate_fluctuation(double x_i, int m); // formula 42
    void accurate_fluctuation(double x_i, int m);   // formula 43, 45

    // internal getters - used for getFluctuation
    double getFluctuation_inaccurate(int m) const;
    double getFluctuation_accurate(int m) const;

};

#endif