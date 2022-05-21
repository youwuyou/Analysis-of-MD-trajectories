#ifndef CORRELATIONEXTENSION_H
#define CORRELATIONEXTENSION_H

#include <Eigen/Core>  // using Eigen
#include <unsupported/Eigen/FFT>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include "TrajectoryFileWriter.h"
#include "BinaryIO.h"


class CorrelationCalculator{

    public:

        CorrelationCalculator(int steps, int N, double delta_t): numMDSteps(steps), numberAtoms(N), timeStep(delta_t){

            // TODO using read-in function to initialize the data

            // C_direct storing the final result
            C_direct.setZero(numMDSteps); // #entries = #positions with property v recorded


            // C_FFT storing the final result, Cx/y/z_FFT storing the contribution in each direction
            C_FFT.setZero(numMDSteps);
            Cx_FFT.setZero(numMDSteps);
            Cy_FFT.setZero(numMDSteps);
            Cz_FFT.setZero(numMDSteps);
        }

        // computation
        void computeCorrelation_direct();
        void computeCorrelation_FFT();


        // read-in
        // TODO implement a wrapper reading in the velocities from the velocities.traj file


        // read-out
        void writeOutCorrelation(const Eigen::VectorXd &C, std::string filename);
        void printCorrelation();


    private:
        /******* private parameters ********/
        int numMDSteps;
        int numberAtoms;
        double timeStep;


        // complex zero padded vector for FFT
        Eigen::VectorXcd vec_padded;

        // results
        Eigen::VectorXd C_direct;   // direct   R^numMDSteps
        Eigen::VectorXd C_FFT;     // complex vector FFT


        // intermediate results for C_FFT
        Eigen::VectorXd Cx_FFT;
        Eigen::VectorXd Cy_FFT;
        Eigen::VectorXd Cz_FFT;


        // data
        Eigen::MatrixXd X;   // dimension:   (3 * numberAtoms) x numMDSteps
                             // data matrix storing a certain type of property
                             // for all items at all time steps


        /******* private methods ********/
        Eigen::VectorXd getC_i(const Eigen::VectorXd& v) const;  // helper for formula 53
};



#endif