/**
 * \file NearestStartPoint3D.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class NearestStartPoint3D
 *
 * @author David Caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef NEARESTSTARTPOINT3D_H
#define NEARESTSTARTPOINT3D_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class NearestStartPoint3D : ShowerRecoModuleBase {

public:

  /// Default constructor
  NearestStartPoint3D() {_name = "NearestStartPoint3D"; }

  /// Default destructor
  ~NearestStartPoint3D() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

};

} // showerreco

#endif
/** @} */ // end of doxygen group

