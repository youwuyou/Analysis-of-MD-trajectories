#include <MDSimulation.h>
#include <iostream>

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
    // MDParameters par = ParameterIO::readParameters(parameterFile); // todo redundant? or type everything in manually?

    // CorrelationCalculator C_Calculator(par.numMDSteps, par.numberAtoms, par.timeStep);
    // CorrelationCalculator C_Calculator(par.numMDSteps, par.numberAtoms, par.timeStep / par.trajectoryOutputINterval);




    return 0;
}
