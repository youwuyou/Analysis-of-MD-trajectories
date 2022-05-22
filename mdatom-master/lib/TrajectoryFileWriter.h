#ifndef TRAJECTORYFILEWRITER_H
#define TRAJECTORYFILEWRITER_H

#include "MDParameters.h"
#include <string>
#include <vector>

/*!
 * This class is responsible for writing out the atom coordinates during a MD simulation.
 * member vars or methods marked as extensions are the ones added for recording the velocity
 * in each step for further computations of the correlation functions
 */
class TrajectoryFileWriter {
public:
    // default constructor
    TrajectoryFileWriter(const MDParameters &parameters,
                         std::string finalCoordFilename,
                         std::string trajFilename);


    // extended constructor for velocities.traj
    TrajectoryFileWriter(const MDParameters &parameters,
                         std::string finalCoordFilename,
                         std::string trajFilename,
                         std::string trajVeloFilename);



    void writeBeforeRun();

    void writeFinalCoordinates(const std::vector<double> &positions, const std::vector<double> &velocities);

    void writeOutTrajectoryStep(const std::vector<double> &positions);

    // extension
    void writeOutVelocityStep(const std::vector<double> &velocities);

    void writeOutVelocityCwise(const std::vector<double> &velocities);



private:
    void writeFinalCoordinatesInBinaryForm(const std::vector<double> &positions, const std::vector<double> &velocities);

    void writeFinalCoordinatesInAsciiForm(const std::vector<double> &positions, const std::vector<double> &velocities);

    void writeOutTrajectoryStepInBinaryForm(const std::vector<double> &positions);

    void writeOutTrajectoryStepInAsciiForm(const std::vector<double> &positions);

    // extension
    void writeOutVelocityInBinaryForm(const std::vector<double> &velocities);

    void writeOutVelocityInAsciiForm(const std::vector<double> &velocities);


    const MDParameters &par;
    const std::string finalCoordinatesFilename;
    const std::string trajectoryCoordinatesFilename;

    // extension
    const std::string trajectoryVelocitiesFilename;
    int counter;
};

#endif // TRAJECTORYFILEWRITER_H
