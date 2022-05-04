#ifndef CLASSEXTENSION_HPP
#define CLASSEXTENSION_HPP

#include <vector>
#include <cmath>

class Calculator{

public:

    Calculator(int numberAtoms, int numberMDSteps): N_x(numberAtoms), numMDSteps(numberMDSteps){}

    double average_slow(const std::vector<double>& x); 
    double fluctuation_slow(const std::vector<double>& x);
    
    // formula 42
    double inaccurate_fluctuation(const std::vector<double>& x);
    
    // pair[0]: average, pair[1]:fluctuation
    std::pair<double, double> AverageFluctuation_fast(const std::vector<double>& x);

    //Correlation function
    std::vector<double> C_direct(const std::vector<double>& x);

private:

    int N_x;
    int numMDSteps;
        
};

#endif