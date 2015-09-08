/**
 * \file StartPoint3DModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class StartPoint3DModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef STARTPOINT3DMODULE_H
#define STARTPOINT3DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class StartPoint3DModule : ShowerRecoModuleBase {

public:

  /// Default constructor
  StartPoint3DModule() {_name = "StartPoint3DModule"; }

  /// Default destructor
  ~StartPoint3DModule() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  void setNSteps(float f){fNSteps = f;}

 /** 
     * @brief generate seed points along 3D axis
     * @details 
     * 
     * 
     * @param seedPoint A point on the 3D axis stored at the end of Axis3DModule 
     * @param nSteps The number of steps to take along the 3D axis 
     * @param resultShower The Shower info passed to do_reconstruction
     * @param result The resulting vector of seeds, returned by reference
     */
  void generatePoints(TVector3 & seedPoint, int nSteps, const Shower_t & resultShower, std::vector<TVector3> & result);

 /** 
     * @brief Find some point along the 3D axis 
     * @details 
     * 
     * 
     * @param anchorCandidates List of candidate 3D points generated using 2D start in Y plane 
     * @param resultShower Want access to the axis-- pass in this fashion.
     * @param nSteps Number of steps to take from Y = -116.5 to 116.5
     * @param plane Plane number 
     */
//  void generateAnchorPoint(TVector3 anchorCandidates, const Shower_t & resultShower, const int & nSteps, const int & plane); 

  bool inTPC( const TVector3 & pt );

private:

    int fNSteps ;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

