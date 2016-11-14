/**
 * \file Angle3DFromVtxQweighted.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class Angle3DFromVtxQweighted
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef ANGLE3DFROMVERTEXQWEIGHTED_H
#define ANGLE3DFROMVERTEXQWEIGHTED_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class Angle3DFromVtxQweighted : ShowerRecoModuleBase {

public:

  /// Default constructor
  Angle3DFromVtxQweighted() {_name = "Angle3DFromVtxQweighted";}

  /// Default destructor
  ~Angle3DFromVtxQweighted() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

