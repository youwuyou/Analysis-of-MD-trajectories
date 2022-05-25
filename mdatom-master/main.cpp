#include <MDSimulation.h>
#include <iostream>

#include <CorrelationExtension.h>
#include <ParameterIO.h>

#include <chrono>
/*!
 * main function.
 * The program expects one argument for the input file (parameters), one optional argument for
 * the file with the initial coordinates.
 */

int main(int argc, char *argv[]) {

    // STAGE 1: Running the MD program, recording properties

    switch (argc) {
        case 2:
            break;
        case 3:
            break;
        default:
            std::cerr << "Usage: mdatom input_file [coordinate_file] > output \n";
            return 1;
    }

    std::string parameterFile = argv[1];
    std::string coordinatesFile; // NB: might be empty
    if (argc > 2)
        coordinatesFile = argv[2];

    MDSimulation md(std::cout);
    try {
        md.performSimulation(parameterFile, coordinatesFile);
    }
    catch (std::exception &e) {
        std::cerr << e.what();
        return 1;
    }


    // STAGE 2:computation of the correlation functions
    MDParameters par = ParameterIO::readParameters(parameterFile);

    CorrelationCalculator C_Calculator(par.numberMDSteps, par.numberAtoms, par.timeStep);

    // computation to be benchmarked
    std::cout << std::endl;
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    C_Calculator.computeCorrelation_direct();
    std::chrono::system_clock::time_point stop = std::chrono::system_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    std::cout << "  Timer spent for computing correlation function using direct method: " << duration_ms << " microsec" << std::endl;

    start = std::chrono::system_clock::now();
    C_Calculator.computeCorrelation_FFT();
    stop = std::chrono::system_clock::now();
    duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    std::cout << "  Timer spent for computing correlation function using FFT method: " << duration_ms << " microsec" << std::endl;

    // write out result to disk
    C_Calculator.writeOutCorrelation();


    return 0;
}
