#include "CorrelationExtension.h"


/****************  PUBLIC *****************/


CorrelationCalculator::CorrelationCalculator(int steps, int n, double stepsize):
                                    numMDSteps(steps), numberAtoms(n), stepSize(stepsize){

    // store data from MD program in RAM
    Mat = Eigen::MatrixXd::Zero(numberAtoms * 3, numMDSteps);
    readInRAM();


    // Storage of the final results
    C_direct.setZero(numMDSteps); // #entries = #positions with property v recorded
    C_FFT.setZero(numMDSteps);


    // coefficient to be divided for FFT
    divisor.setLinSpaced(numMDSteps, 0, numMDSteps);

    // 2nd entry is the wanted constant
    Eigen::VectorXd Nmd_constant = Eigen::VectorXd::Constant(numMDSteps, numMDSteps);

    divisor = Nmd_constant - divisor;
    divisor = 2. * divisor.cwiseProduct(Nmd_constant);
}


// formula 49 - x is some property stored in a matrix while the MD program is running

void CorrelationCalculator::computeCorrelation_direct(){

    std::cout << "calculating correlation directly" << std::endl;

    double S;

    // looping through all steps
    for(int n = 0; n < numMDSteps; ++n){
        S = 0.;

        // computation for each property component
        for(int k = 0; k < numMDSteps - n; ++k){
            S += (Mat.col(k)).dot(Mat.col(k + n));
        }

        C_direct(n) = S / ( numMDSteps - n);
    }

    C_direct /= C_direct(0);   // normalization
}



// formula 53
Eigen::VectorXd CorrelationCalculator::getC_i(const Eigen::VectorXd& v) {

    int size = v.size();

    // zero padding
    Eigen::VectorXcd vec_padded;

    vec_padded.setZero(size * 2);
    vec_padded.head(size) = v;

    // FFT
    Eigen::FFT<double> fft;
    auto temp = fft.fwd(vec_padded);
    auto transformed = temp.cwiseProduct(temp.conjugate()).eval(); // squared modulus

    // IFFT
    return fft.inv(transformed).real().head(size);
}



void CorrelationCalculator::computeCorrelation_FFT(){

    std::cout << "calculating correlation using FFT" << std::endl;

    // looping through all atoms
    for(int k = 0; k < numberAtoms; ++k){
        C_FFT += getC_i(Mat.row(k).transpose());
    }
        C_FFT = C_FFT.cwiseQuotient(divisor);
        C_FFT /= C_FFT(0);   // normalization

}



void CorrelationCalculator::writeOutCorrelation(){
    writeToDisk(std::move(C_direct), "c_direct.txt");
    writeToDisk(std::move(C_FFT), "c_fft.txt");
}



/****************  PRIVATE *****************/

// Input
void CorrelationCalculator::readInRAM(){

    std::cout << "Read in data from files storing velocities during MD" << std::endl;

    int lines = numberAtoms * numMDSteps;
    std::vector<double> data_x(lines), data_y(lines), data_z(lines); // vector storing points to 3 raw data arrays


    // use the internal file reader to read data, which is stored in columns
    getDataFromFile(data_x, data_y, data_z);


    // reshape vectors into matrices
    Eigen::MatrixXd X = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_x.data(), numberAtoms, numMDSteps);
    Eigen::MatrixXd Y = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_y.data(), numberAtoms, numMDSteps);
    Eigen::MatrixXd Z = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> (data_z.data(), numberAtoms, numMDSteps);

    // store matrices X, Y, Z blockwise using a big data matrix "Mat"
    Mat << X, Y, Z;
    std::cout << "successfully initialized the big data matrix" << std::endl;
}



void CorrelationCalculator::getDataFromFile(std::vector<double>& data_x,
                                                std::vector<double>& data_y,
                                                    std::vector<double>& data_z){

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

    std::size_t lines = data_x.size();

    for(std::size_t i = 0; i < lines; ++i){
        fin_x >> data_x.at(i);
        fin_y >> data_y.at(i);
        fin_z >> data_z.at(i);
    }

}


// Output
void CorrelationCalculator::writeToDisk(const Eigen::VectorXd &C, std::string filename){
    std::ofstream fileFW;

    // debugging
    std::cout << "writting out correlations to txt file" << std::endl;

    fileFW.open(filename, std::ios::out | std::ios::app);
    if (fileFW.bad()) {
        throw std::runtime_error("I/O ERROR: cannot write to file: " + filename);
    }

    // fileFW << "\n\n Correlation function C(t) :\n\n"
    //     << "        step             C(t)\n";

    for (int i = 0; i < numMDSteps; i++) {
        fileFW << std::setw(10) << i;
        fileFW << std::setw(15) << C(i) << "\n";
    }
    fileFW << "\n\n";
    fileFW.close();
}
