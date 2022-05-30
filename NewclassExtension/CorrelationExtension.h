#ifndef CORRELATIONEXTENSION_H
#define CORRELATIONEXTENSION_H

#include <Eigen/Core>  // using Eigen
#include <unsupported/Eigen/FFT>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <exception>
#include "TrajectoryFileWriter.h"
#include "BinaryIO.h"



class CorrelationCalculator{

    public:

        // constructor
        CorrelationCalculator(int steps, int n, double stepsize);


        // computation
        void computeCorrelation_direct();
        void computeCorrelation_FFT();
        void computeCorrelation_FFT(bool complete);

        Eigen::VectorXd getC_i(const Eigen::VectorXd& v);  // helper for formula 53



        // read-out
        void writeOutCorrelation();


    private:
        /******* private parameters ********/
        int numMDSteps;
        int numberAtoms;
        double stepSize;
        int N;


        // helpers for FFT
        Eigen::VectorXd divisor;      // coeff to be divided
        Eigen::VectorXd averages;     // for bias


        // results
        Eigen::VectorXd C_direct;   // direct   R^numMDSteps
        Eigen::VectorXd C_FFT;     // complex vector FFT

        // data
        Eigen::MatrixXd Mat;   // dimension:   (numberAtoms * 3) x numMDSteps
                               // data matrix storing a certain type of property
                               // for all items at all time steps


        /******* private methods ********/
        // I/O
        void readInRAM();   // wrapper script for reading in
        void getDataFromFile(std::vector<double>& data_x,
                              std::vector<double>& data_y,
                                std::vector<double>& data_z);

        void writeToDisk(const Eigen::VectorXd &C, std::string filename);

};



#endif