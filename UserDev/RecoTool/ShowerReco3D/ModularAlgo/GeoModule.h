/**
 * \file GeoModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class GeoModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef GEOMODULE_H
#define GEOMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class GeoModule : ShowerRecoModuleBase {

public:

  /// Default constructor
  GeoModule() {_name = "GeoModule";}

  /// Default destructor
  ~GeoModule() {}


  void do_reconstruction(const ProtoShower &, Shower_t &);

private:

  // Make sure the axis is pointing in the right direction
  void checkAxisDir(const ShowerClusterSet_t & inClusters, Shower_t & result);

  // Length:
  void fillLength(const ShowerClusterSet_t & inClusters, Shower_t & result);

  // Get the opening angle:
  void fillAngle(const ShowerClusterSet_t & inClusters, Shower_t & result);

};

} // showerreco

#endif
/** @} */ // end of doxygen group

