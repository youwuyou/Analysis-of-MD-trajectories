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

        CorrelationCalculator(int steps, int N, double delta_t): numMDSteps(steps), numberAtoms(N), timeStep(delta_t){

            // TODO using read-in function to initialize the data
            Mat = Eigen::MatrixXd::Zero(numberAtoms * 3, numMDSteps);

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
        // implement a wrapper reading in the velocities from the velocities.traj file
        void readInCorrelation(){

            double *data_x, *data_y, *data_z; // vector storing points to 3 raw data arrays


            // use the internal file reader to read data, which is stored in columns
            getDataFromFile(data_x, data_y, data_z);

            // get raw data from "velocities.traj"

            // reshape the vector into a matrix
            auto X = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_x, numberAtoms, numMDSteps);
            auto Y = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_y, numberAtoms, numMDSteps);
            auto Z = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_z, numberAtoms, numMDSteps);


            // store matrices X, Y, Z blockwise using a big data matrix "Mat"
            Mat << X, Y, Z;
        }


        // TODO: perhaps change the type and see how to read from "velocities.traj"
        void getDataFromFile(double *data_x,
                                double *data_y,
                                    double *data_z);


// void readIn(std::vector<double> data, std::string filename)
// {
//     std::ifstream fin;
//     fin.open(filename, std::ios::in);
//         if (fin.bad()) {
//             throw std::runtime_error("can't open " + filename);
//         }

//     double buffer;
//     int lines = numberAtoms * numMDSteps;

//     for (int i = 0; i < lines; ++i) {
//         // Read in the velocities
//         fin >> buffer;

//         // store values in
//         data.at(i) = buffer;
//     }
// }


// void BinaryIO::readPtr(std::istream &in, double* &array) {
//     size_t length = array.size();
//     in.read(reinterpret_cast<char *>(array), length * sizeof(double));
// }


// void BinaryIO::read(std::istream &in, std::vector<double> &array) {
//     size_t length = array.size();
//     in.read(reinterpret_cast<char *>(array.data()), length * sizeof(double));
// }


// void CenterOfMassCalculator::getPosition(double *x) const {
//     for (int i = 0; i < 3; ++i)
//         x[i] = xcm[i];
// }



        // read-out
        void writeOutCorrelation(const Eigen::VectorXd &C, std::string filename);
        void printCorrelation();


    private:
        /******* private parameters ********/
        int numMDSteps;
        int numberAtoms;
        double timeStep;


        // zero padded vector for FFT
        Eigen::VectorXcd vec_padded;

        // results
        Eigen::VectorXd C_direct;   // direct   R^numMDSteps
        Eigen::VectorXd C_FFT;     // complex vector FFT


        // intermediate results for C_FFT
        Eigen::VectorXd Cx_FFT;
        Eigen::VectorXd Cy_FFT;
        Eigen::VectorXd Cz_FFT;
        Eigen::VectorXd divisor;


        // data
        Eigen::MatrixXd Mat;   // dimension:   (numberAtoms * 3) x numMDSteps
                               // data matrix storing a certain type of property
                               // for all items at all time steps


        /******* private methods ********/
        Eigen::VectorXd getC_i(const Eigen::VectorXd& v);  // helper for formula 53
};



#endif