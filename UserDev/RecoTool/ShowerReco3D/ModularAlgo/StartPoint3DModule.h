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
  StartPoint3DModule() {_name = "StartPoint3DModule"; _res = 0.5 ; }

  /// Default destructor
  ~StartPoint3DModule() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

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

  bool inTPC( const TVector3 & pt );

  void setResolution ( double res ) { _res = res; }

  private:

  double _res ;


};

} // showerreco

#endif
/** @} */ // end of doxygen group

