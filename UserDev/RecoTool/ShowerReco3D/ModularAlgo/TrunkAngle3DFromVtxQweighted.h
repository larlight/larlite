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
#ifndef TRUNKANGLE3DFROMVERTEXQWEIGHTED_H
#define TRUNKANGLE3DFROMVERTEXQWEIGHTED_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
*/
namespace showerreco {

  class TrunkAngle3DFromVtxQweighted : ShowerRecoModuleBase {

  public:

    /// Default constructor
    TrunkAngle3DFromVtxQweighted();

    /// Default destructor
    ~TrunkAngle3DFromVtxQweighted() {}

    void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

    // set distance along which to calculate the angle
    void setTrunkLength(double d) { _dtrunk = d; }

  private:

    double _dtrunk;

  };

} // showerreco

#endif
/** @} */ // end of doxygen group

