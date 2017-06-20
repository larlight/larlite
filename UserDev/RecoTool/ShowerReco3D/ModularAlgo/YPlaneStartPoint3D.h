/**
 * \file YPlaneStartPoint3D.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class YPlaneStartPoint3D
 *
 * @author David Caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef YPLANESTARTPOINT3D_H
#define YPLANESTARTPOINT3D_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class YPlaneStartPoint3D : ShowerRecoModuleBase {

public:

  /// Default constructor
  YPlaneStartPoint3D() {_name = "YPlaneStartPoint3D"; }

  /// Default destructor
  ~YPlaneStartPoint3D() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

};

} // showerreco

#endif
/** @} */ // end of doxygen group

