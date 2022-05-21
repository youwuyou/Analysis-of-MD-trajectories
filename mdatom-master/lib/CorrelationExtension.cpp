#include "CorrelationExtension.h"


/****************  PUBLIC *****************/
// formula 49 - x is some property stored in a matrix while the MD program is running
void CorrelationCalculator::computeCorrelation_direct(){

    // looping through all steps
    double S;
    for(int n = 0; n < numMDSteps; ++n){
        S = 0.;

        // computation for each property component
        for(int k = 0; k < numMDSteps - n; ++k){
            S += X.col(k).dot(X.col(k + n));
        }

        C_direct(n) = S / ( numMDSteps - n); // store accumulated correlation in the class member
                                             // of type Eigen::VectorXd for each time point of recording
    }

}



void CorrelationCalculator::computeCorrelation_FFT(){

    // looping through all atoms
    for(int k = 0; k < numberAtoms; ++k){

        Cx_FFT += getC_i(X.row(3 * k));
        Cy_FFT += getC_i(X.row(3 * k + 1));
        Cz_FFT += getC_i(X.row(3 * k + 2));

    }

    C_FFT = (  Cx_FFT
             + Cy_FFT
             + Cz_FFT );  // adding up contribution in each direction
                          // divide up by coefficients
}



void CorrelationCalculator::writeOutCorrelation(const Eigen::VectorXd &C, std::string filename){
    std::ofstream fileFW;
    fileFW.open("correlations.txt", std::ios::out | std::ios::app);
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