#ifndef CLASSEXTENSION_HPP
#define CLASSEXTENSION_HPP

#include <vector>
#include <cmath>

class Calculator{

public:

    Calculator(std::vector<double> veloc): x_(veloc), N_x(veloc.size()){}

    double average_slow(); 
    double fluctuation_slow();
    
    // pair[0]: average, pair[1]:fluctuation
    std::pair<double, double> AverageFluctuation_fast();

private:

    std::vector<double> x_;
    int N_x;
        
};

#endif