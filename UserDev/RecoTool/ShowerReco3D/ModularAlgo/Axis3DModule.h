/**
 * \file Axis3DModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class Axis3DModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef AXIS3DMODULE_H
#define AXIS3DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

enum Status {kNormal, kNotOptFit, kTwoPlane, kAveraged, kIterMaxOut, kNStatus};

class SeedVector {

public:

    bool     _exists = false;
    TVector3 _vector;
    float    _error;
    Status   _status;
    int      _iterations;

    /// Default constructor
    SeedVector() {}

    /// Constructor
    SeedVector(TVector3 vector, float error) {_exists = true; _vector = vector; _error = error;}

    /// Copy constructor
    SeedVector(const SeedVector & vec) {
        _exists     = vec._exists;
        _vector     = vec._vector;
        _error      = vec._error;
        _status     = vec._status;
        _iterations = vec._iterations;
    }

    /// Default destructor
    ~SeedVector() {}

};

class Axis3DModule : ShowerRecoModuleBase {

public:

    /// Default constructor
    Axis3DModule() {_name = "Axis3DModule";}

    /// Default destructor
    ~Axis3DModule() {}


    void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

    /**
     * @brief Set the value of maximum number of iterations per shower
     *
     * @param i Max Iterations
     */
    void setMaxIterations(int i) {fMaxIterations = i;}
    /**
    * @brief Set the value of step size used to generate seed vectors (see generateSeedVectors)
    *
    * @param f Minimum Step Size
    */
    void setNStepsInitial(float f) {fNStepsInitial = f;}
    /**
     * @brief Set the target error in the projected, squared difference between 3D axis and planes
     *
     * @param f target error
     */
    void setTargetError(float f) {fTargetError = f;}

    void setNormalErrorRange(float f) {_normalErrorRange = f;}

    /**
     * @brief sets the starting value at which theta converges
     * @details Theta range is the opening angle around which the algorithm searches
     *
     * @param f target theta range
     */
    void setThetaRangeStart(float f) {_thetaRangeStart = f;}

    /**
     * @brief sets the  value at which theta stops decreasing
     *
     * @param f target theta min
     */
    void setThetaRangeMin(float f) {_thetaRangeMin = f;}


    /**
     * @brief Set the number of steps to use when finding better vectors
     * @details More steps is slower, but also might be more accurate.  Alg will stop converging
     *          after 4* the start steps.
     *
     * @param n starting number of steps
     */
    void setNStepsStart(size_t n) {_nStepsStart = n;}

    /**
     * @brief Set the rate at which the angle decreases
     * @details This algorithm will decrease the range it's looking in as it
     *          converges.  This is the rate at which it decreases.  Don't make it too small.
     *
     * @param f Target convergence rate, default is ~0.85
     */
    void setConvergeRate(float f) {_alpha = f;}

    void setSeedVectorErrorCutoff(float f) {_seedVectorErrorCutoff = f;}

private:

    /**
     * @brief generate seed direction vectors in the angle around the initial vector
     * @details Takes the initial vector and maps out and area around it and returns that vector array
     *          It defines theta as the angle outward from the initial vector.  There is no reason
     *          for theta to be greater than 90 degrees because this will make a full circuit in phi
     *          and there is no distinction later amongst vectors and their inverse
     *
     *
     * @param initialVector The TVector3 that is the effective Z axis, all the others wrap around it.
     * @param thetaRange The range downward from which the output vectors will reach in direction
     * @param nSteps The number of steps to take in each 2PI pass and also the number of steps to
     *               break up the theta range into.
     * @param result The resulting vector of seeds, returned by reference
     */
    void generateSeedVectors(const TVector3 & initialVector,
                             float thetaRange,
                             int nSteps,
                             std::vector<TVector3> & result);

    /**
     * @brief make a set of vectors around the initial vector
     * @details Makes a set of vectors that wrap around the initial vector but offset at an angle thetaRange.
     *          Makes a number of vectors = nsteps, returns by reference through result
     *
     * @param initialVector the input vector as the see
     * @param thetaRange the angle at which vectors will be offset
     * @param nSteps The number of evenly spaced vectors to create
     * @param result the result
     */
    void generateNearbyVectors( const TVector3 & initialVector,
                                float thetaRange,
                                int nSteps,
                                std::vector<TVector3> & result);

    /**
     * @brief Loops over every vector in the seedVectors and computes the error for that vector
     * @details Projects every vector into every plane and computes the error in the slope.  Returns the
     *          index of the vector that has the smallest computed error.  In future, might return
     *          an array of indexes to allow searching over local minimums ...
     *
     * @param slopesByPlane The target slope in each plane
     * @param planes The index of each plane - doesn't assume we're getting planes in any particular order
     * @return The index in the seedVectors array of the best vector
     */
    int findBestVector(const std::vector<TVector3> seedVectors,
                       const std::vector<float> & slopesByPlane,
                       const std::vector<int> & planes);


    /**
     * @brief Finds the projected error of this vector compared to the input planes
     * @details Projects the vector into the plane, then computes the error (actually, the
     *          error squared) of the slope_target - slope_projected summed over planes
     *
     * @param inputVector The vector in 3D to test
     * @param slopesByPlane The target slope in each plane
     * @param planes The index of each plane - doesn't assume we're getting planes in any particular order
     *
     * @return The error (squared), unitless
     */
    float getErrorOfProjection(const TVector3 & inputVector,
                               const std::vector<float> & slopesByPlane,
                               const std::vector<int> & planes);

    /**
     * @brief This function takes a seed vector and optimizes it to the slopes and planes specified
     * @details The algorithm will make a set of vectors that surround the seed vector and move the seed
     *          to the vector that has the least error.  If the error is smallest at the seed vector,
     *          it attempts to decrease the window around the vector to reduce the area it's looking in.
     *          If the calculated error (which is normalized to nPlanes) drops below a required minimum,
     *          the algorithm returns with exit status kNormal.  If the window around the algorithm
     *          reaches a minimum window without the minimum error, the algorithm also returns but with
     *          exit status kNotOptFit.  If the algorithm fails to converge in the max number of iterations,
     *          it returns with status kIterMaxOut.
     *
     * @param inputVector Reference to the initial seed vector, gets modified directly
     * @param exitStatus Reference to the exit status, filled as enum Status
     * @param n_iterations The number of iterations this algorithm required
     * @param slopeByPlane The slopes that this algorithm is going to fit against
     * @param plane The planes that each of the above slopes match to.
     * @return The error of the performed fit.
     */
    float optimizeVector(TVector3 & inputVector,
                         Status & exitStatus,
                         int & n_iterations,
                         const std::vector<float> & slopeByPlane,
                         const std::vector<int> & planes );

    /**
     * @brief Identifies good seed vector candidates from the initial globalSeedVectors list
     * @details The algorithm will loop over each vector in the globalSeedVectors list. For each one, it will
     *          get the error provided by that projection and compare it to some initial value. All vectors
     *          that satisfy this condition will be added to the vector of candidates. If no vectors satisfy
     *          the initial condition, the code will relax this condition slightly and try again, until it gets
     *          at least one candidate.
     *
     * @param seedVectors Reference to the empty seed vector to which candidates are added
     * @param planes The planes that each of the slopes match to
     * @param slopeByPlane The slopes on each plane that the algorithm fits against
     */
    void findSeedVectors(std::vector<SeedVector> & seedVectors,
                         const std::vector<int> & planes,
                         const std::vector<float> & slopeByPlane);

    /**
     * @brief Optimizes all seed vectors contained in input vector
     * @details The algorithm takes an input vector which contains all candidates for a vector. It then attempts
     *          to refine each one using the OptimizeVector function, and then logs some information, such as
     *          whether the optimization converged, and how good the final fit was.
     *
     * @param seedVectors Reference to the vector containing all potential seed candidates
     * @param planes The planes that each of the slopes match to
     * @param slopeByPlane The slope on each plane that the algorithm fits against
     */
    void optimizeSeedVectors(std::vector<SeedVector> & seedVectors,
                             const std::vector<int> & planes,
                             const std::vector<float> & slopeByPlane);

    /**
     * @brief Prints out all candidate vectors, and their errors
     * @details Takes in a vector of all candidate seed vectors, and prints out the error and vector for each one,
     *          as well as the status of the optimization.
     *
     * @param seedVectors Reference to the vector containing all potential seed vector candidates
     * @param planes The planes that each of the slopes match to
     * @param slopeByPlane The slope on each plane that the algorithm fits against
     */
    void printVectors(std::vector<SeedVector> & seedVectors,
                      const std::vector<int> & planes,
                      const std::vector<float> & slopeByPlane);

    /**
     * @brief Finds the one optimal vector from the vector of candidates
     * @details Algorithm which loops through the vector containing all candidates, and checks the error on each one.
     *          It then saves the index & error of the one vector with the smallest error, and saves them in bestIndex
     *          and bestError.
     *
     * @param seedVectors Reference to the object containing all potential seed vector candidates
     * @return The optimal vector candidate
     */
    SeedVector findOptimalVector(std::vector<SeedVector> & seedVectors);

    // Info on best vector (ie. module output) for writing to tree
    bool  _vec_exists;
    float _vec_error;
    int   _vec_status;
    float _vec_x;
    float _vec_y;
    float _vec_z;
    float _slope_0;
    float _slope_1;
    float _slope_2;
    float _slope_0_true;
    float _slope_1_true;
    float _slope_2_true;

    // Function to initialise output tree
    void initialize();

    // Private members that get used frequently:
    std::vector<TVector3> _globalSeedVectors;


    // Configurable variables for this module:

    // Maximum number of iterations to loop over per seed vector
    int fMaxIterations;
    // The initial number of divisions used for computing seed vectors
    float fNStepsInitial;
    // The target error (fractional per plane) for normal completion
    float fTargetError;

    float _normalErrorRange;

    // The cutoff error (fractional error per plane) for picking seed vectors
    float _seedVectorErrorCutoff;

    // The initial opening angle for a seed vector
    float _thetaRangeStart;

    float _thetaRangeMin;
    // float _thetaRangeMax   = M_PI / 8;

    // The number of steps that are used in wrapping around a seed vector
    int _nStepsStart;

    // Alpha is the "converge rate" - the rate at which the angle narrows in on a value
    float _alpha;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

