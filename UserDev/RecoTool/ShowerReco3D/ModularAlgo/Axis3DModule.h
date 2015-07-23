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

  class Axis3DModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    Axis3DModule(){_name = "Axis3DModule";}

    /// Default destructor
    ~Axis3DModule(){}


    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

    /**
     * @brief Set the value of maximum number of iterations per shower
     * 
     * @param i Max Iterations
     */
    void setMaxIterations(int i){fMaxIterations = i;}
        /**
     * @brief Set the value of step size used to generate seed vectors (see generateSeedVectors)
     * 
     * @param f Minimum Step Size
     */
    void setNStepsMin(float f){fNStepsMin = f;}
    /**
     * @brief Set the target error in the projected, squared difference between 3D axis and planes
     * 
     * @param f target error
     */
    void setTargetError(float f){fTargetError = f;}

  private:

    /**
     * @brief generate seed direction vectors in the angle around the initial vector
     * @details Takes the initial vector and maps out and area around it and returns that vector array
     * It defines theta as the angle outward from the initial vector.  There is no reason for theta to be 
     * greater than 90 degrees because this will make a full circuit in phi and there is no distinction later 
     * amongst vectors and their inverse
     * 
     * 
     * @param initialVector The TVector3 that is the effective Z axis, all the others wrap around it.
     * @param thetaRange The range downward from which the output vectors will reach in direction
     * @param nSteps The number of steps to take in each 2PI pass and also the number of steps to 
     *               break up the theta range into.
     * @param result The resulting vector of seeds, returned by reference
     */
    void generateSeedVectors(const TVector3 & initialVector, float thetaRange, int nSteps, std::vector<TVector3> & result);

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
    int findBestVector(const std::vector<TVector3> seedVectors, const std::vector<float> & slopesByPlane, const std::vector<int> & planes);


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
    float getErrorOfProjection(const TVector3 & inputVector, const std::vector<float> & slopesByPlane, const std::vector<int> & planes);


    int fMaxIterations;
    float fNStepsMin;
    float fTargetError;

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

