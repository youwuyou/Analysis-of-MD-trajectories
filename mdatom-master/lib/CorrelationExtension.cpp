#include "CorrelationExtension.h"


/****************  PUBLIC *****************/
// formula 49 - x is some property stored in a matrix while the MD program is running
void CorrelationCalculator::computeCorrelation_direct(){

    std::cout << "calculating correlation directly" << std::endl;
    // looping through all steps
    double S;
    for(int n = 0; n < numMDSteps; ++n){
        S = 0.;

        // computation for each property component
        for(int k = 0; k < numMDSteps - n; ++k){
            S += Mat.col(k).dot(Mat.col(k + n));
        }

        C_direct(n) = S / ( numMDSteps - n); // store accumulated correlation in the class member
                                             // of type Eigen::VectorXd for each time point of recording
    }

}



void CorrelationCalculator::computeCorrelation_FFT(){

    std::cout << "calculating correlation using FFT" << std::endl;

    // looping through all atoms
    for(int k = 0; k < numberAtoms; ++k){

        Cx_FFT += getC_i(Mat.row(3 * k));
        Cy_FFT += getC_i(Mat.row(3 * k + 1));
        Cz_FFT += getC_i(Mat.row(3 * k + 2));

    }

    C_FFT = (  Cx_FFT
             + Cy_FFT
             + Cz_FFT ).cwiseQuotient(divisor);  // adding up contribution in each direction
                                                 // divide up by coefficients
}


// read-in
// implement a wrapper reading in the velocities from the velocities.traj file
void CorrelationCalculator::readInCorrelation(){

    std::cout << "Read in data from files storing velocities during MD" << std::endl;

    int lines = numberAtoms * numMDSteps;
    std::vector<double> data_x(lines), data_y(lines), data_z(lines); // vector storing points to 3 raw data arrays


    // use the internal file reader to read data, which is stored in columns
    getDataFromFile(data_x, data_y, data_z);

    // get raw data from "velocities.traj"

    // reshape the vector into a matrix
    auto X = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_x.data(), numberAtoms, numMDSteps);
    auto Y = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_y.data(), numberAtoms, numMDSteps);
    auto Z = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_z.data(), numberAtoms, numMDSteps);


    // store matrices X, Y, Z blockwise using a big data matrix "Mat"
    Mat << X, Y, Z;
    std::cout << "successfully initialized the big data matrix" << std::endl;

}


void CorrelationCalculator::getDataFromFile(std::vector<double>& data_x,
                                                std::vector<double>& data_y,
                                                    std::vector<double>& data_z){

    BinaryIO input; // class instance for inputting data

    // open the "velocities.traj" file
    // create input stream for the cols


    std::ifstream fin_x;
    fin_x.open("velo_x.traj", std::ios::in);
        if (fin_x.bad()) {
            throw std::runtime_error("can't open velo_x.traj");
        }

    std::ifstream fin_y;
    fin_y.open("velo_y.traj", std::ios::in);
        if (fin_y.bad()) {
            throw std::runtime_error("can't open velo_y.traj");
        }

    std::ifstream fin_z;
    fin_z.open("velo_z.traj", std::ios::in);
        if (fin_z.bad()) {
            throw std::runtime_error("can't open velo_z.traj");
        }


    std::cout << "read in the data" << std::endl;


    // read in the data
    input.read(fin_x, data_x);  // x-component <-> 2nd col
    input.read(fin_y, data_y);  // y-component <-> 3rd col
    input.read(fin_z, data_z);  // z-component <-> 4th col

}





void CorrelationCalculator::writeOutCorrelation(const Eigen::VectorXd &C, std::string filename){
    std::ofstream fileFW;

    // debugging
    std::cout << "writting out correlations to txt file" << std::endl;

    fileFW.open(filename, std::ios::out | std::ios::app);
    if (fileFW.bad()) {
        throw std::runtime_error("I/O ERROR: cannot write to file: " + filename);
    }

    fileFW << "\n\n Correlation function C(t) :\n\n"
        << "        step             C(t)\n";

    for (int i = 0; i < numMDSteps; i++) {
        fileFW << std::setw(10) << i;
        fileFW << std::setw(15) << C(i) << "\n";
    }
    fileFW << "\n\n";
    fileFW.close();
}



void CorrelationCalculator::printCorrelation(){
    writeOutCorrelation(std::move(C_direct), "c_direct.txt");
    writeOutCorrelation(std::move(C_FFT), "c_fft.txt");
}



/****************  PRIVATE *****************/
// formula 53
Eigen::VectorXd CorrelationCalculator::getC_i(const Eigen::VectorXd& v) {

    int size = v.size();

    // zero padding
    vec_padded.setZero(2 * size);
    vec_padded.head(size) = v;


    // helper
    Eigen::FFT<double> fft;
    vec_padded = fft.fwd(vec_padded); // applied Fourier matrix on the given vector

    auto intermediate = (vec_padded.conjugate()).cwiseProduct(vec_padded).eval(); // no copying using eval


    return fft.inv(intermediate).real().head(size) / (2 * size); // applied inverse Fourier matrix
                                                    // and cut off the zero padded parts
}