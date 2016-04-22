/**
 * \file Angle3DFromVtx.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class Angle3DFromVtx
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef ANGLE3DFROMVERTEX_H
#define ANGLE3DFROMVERTEX_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class Angle3DFromVtx : ShowerRecoModuleBase {

public:

  /// Default constructor
  Angle3DFromVtx() {_name = "Angle3DFromVtx";}

  /// Default destructor
  ~Angle3DFromVtx() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

