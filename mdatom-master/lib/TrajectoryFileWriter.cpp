#include "TrajectoryFileWriter.h"
#include "BinaryIO.h"
#include "CoordinatesAndVelocitiesInitializer.h" // For MAXTITLE value
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <utility>

using namespace std;

/****************************  ORIGINAL *****************************/
// default constructor
TrajectoryFileWriter::TrajectoryFileWriter(const MDParameters &parameters,
                                           std::string finalCoordFilename,
                                           std::string trajFilename)
        : par(parameters),
          finalCoordinatesFilename(std::move(finalCoordFilename)),
          trajectoryCoordinatesFilename(std::move(trajFilename)) ,
          trajectoryVelocitiesFilename(""),
          counter(0){
}

void TrajectoryFileWriter::writeBeforeRun() {
    ofstream fout1; // trajectory output
    if (par.trajectoryOutput) {
        fout1.open(trajectoryCoordinatesFilename, ios::out);
        if (fout1.bad()) {
            throw std::runtime_error("can't open " + trajectoryCoordinatesFilename);
        }
        fout1 << par.title << endl;
    }
}

void TrajectoryFileWriter::writeFinalCoordinates(const std::vector<double> &positions,
                                                 const std::vector<double> &velocities) {
    if (par.finalXVOutput == FinalCoordinateFileFormat::ascii) {
        writeFinalCoordinatesInAsciiForm(positions, velocities);
    } else {
        writeFinalCoordinatesInBinaryForm(positions, velocities);
    }
}

void TrajectoryFileWriter::writeFinalCoordinatesInBinaryForm(const std::vector<double> &positions,
                                                             const std::vector<double> &velocities) {
    ofstream fout2;
    fout2.open(finalCoordinatesFilename, ios::out | ios::binary);
    if (fout2.bad()) {
        throw std::runtime_error("can't open " + finalCoordinatesFilename);
    }
    fout2.write(par.title.c_str(), MAXTITLE);
    BinaryIO::write(fout2, positions);
    BinaryIO::write(fout2, velocities);
}

void TrajectoryFileWriter::writeFinalCoordinatesInAsciiForm(const std::vector<double> &positions,
                                                            const std::vector<double> &velocities) {
    ofstream fout2;
    fout2.open(finalCoordinatesFilename, ios::out);
    if (fout2.bad()) {
        throw std::runtime_error("can't open " + finalCoordinatesFilename);
    }
    fout2 << par.title << "\n";
    fout2 << par.numberAtoms << "\n";
    for (int j = 0; j < par.numberAtoms; j++) {
        fout2 << setw(6) << j;
        for (int m = 0; m < 3; m++) {
            fout2 << setw(15) << positions[3 * j + m];
        }
        for (int m = 0; m < 3; m++) {
            fout2 << setw(15) << velocities[3 * j + m];
        }
        fout2 << "\n";
    }
}

void TrajectoryFileWriter::writeOutTrajectoryStep(const std::vector<double> &positions) {
    if (par.trajectoryOutput) {
        if (par.trajectoryOutputFormat == TrajectoryFileFormat::binary) {
            writeOutTrajectoryStepInBinaryForm(positions);
        } else if (par.trajectoryOutputFormat == TrajectoryFileFormat::ascii) {
            writeOutTrajectoryStepInAsciiForm(positions);
        }
    }
}

void TrajectoryFileWriter::writeOutTrajectoryStepInBinaryForm(const std::vector<double> &positions) {
    ofstream fileBW;
    fileBW.open(trajectoryCoordinatesFilename, ios::out | ios::app | ios::binary);
    if (fileBW.bad()) {
        throw runtime_error("I/O ERROR: cannot write to file: " + trajectoryCoordinatesFilename);
    }
    BinaryIO::write(fileBW, positions);
}

void TrajectoryFileWriter::writeOutTrajectoryStepInAsciiForm(const std::vector<double> &positions) {
    ofstream fileFW;
    fileFW.open(trajectoryCoordinatesFilename, ios::out | ios::app);
    if (fileFW.bad()) {
        throw runtime_error("I/O ERROR: cannot write to file: " + trajectoryCoordinatesFilename);
    }
    int ntot = 3 * par.numberAtoms;
    for (int i = 0; i < ntot; i += 10) {
        for (int j = i; (j < i + 10 && j < ntot); j++) {
            fileFW << setw(10) << positions[j];
        }
        fileFW << endl;
    }
}


/****************************  EXTENDED *****************************/
// used for recording velocities for further correlation computation
TrajectoryFileWriter::TrajectoryFileWriter(const MDParameters &parameters,
                                           std::string finalCoordFilename,
                                           std::string trajFilename,
                                           std::string trajVeloFilename)
        : par(parameters),
          finalCoordinatesFilename(std::move(finalCoordFilename)),
          trajectoryCoordinatesFilename(std::move(trajFilename)) ,
          trajectoryVelocitiesFilename(std::move(trajVeloFilename)),
          counter(0){
}



/****************  PUBLIC *****************/

void TrajectoryFileWriter::writeOutVelocityStep(const std::vector<double> &velocities) {
    if (par.trajectoryOutput) {
        if (par.trajectoryOutputFormat == TrajectoryFileFormat::binary) {
            writeOutVelocityInBinaryForm(velocities);
        } else if (par.trajectoryOutputFormat == TrajectoryFileFormat::ascii) {
            writeOutVelocityInAsciiForm(velocities);
        }
    }
}

/****************  PRIVATE *****************/

void TrajectoryFileWriter::writeOutVelocityInBinaryForm(const std::vector<double> &velocities) {
    ofstream fileBW;
    fileBW.open(trajectoryVelocitiesFilename, ios::out | ios::app | ios::binary);
    if (fileBW.bad()) {
        throw runtime_error("I/O ERROR: cannot write to file: " + trajectoryVelocitiesFilename);
    }
    BinaryIO::write(fileBW, velocities);
}

void TrajectoryFileWriter::writeOutVelocityInAsciiForm(const std::vector<double> &velocities) {
    ofstream fileFW;
    fileFW.open(trajectoryVelocitiesFilename, ios::out | ios::app);
    if (fileFW.bad()) {
        throw runtime_error("I/O ERROR: cannot write to file: " + trajectoryVelocitiesFilename);
    }
    int ntot = 3 * par.numberAtoms;   // 3 components for the velocity vector of each atom
    for (int i = 0; i < ntot; i += 3) {
        fileFW << counter << setw(10) << i - 3;   // divide by 3 for writting down current no.atom
                                                  // i = no.atom in the current step

        for (int j = i; (j < i + 3 && j < ntot); j++) {
            fileFW << setw(10) << velocities[j];
        }
        fileFW << endl;
    }

    ++counter;
}