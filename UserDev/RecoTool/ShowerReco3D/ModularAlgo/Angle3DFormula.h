/**
 * \file Angle3DFormula.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class Angle3DFormula
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef ANGLE3DFORMULA_H
#define ANGLE3DFORMULA_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class Angle3DFormula : ShowerRecoModuleBase {

public:

  /// Default constructor
  Angle3DFormula();

  /// Default destructor
  ~Angle3DFormula() {}

  /// set the maximum angle that is allowed as an error (radians)
  void setMaxAngleError(double err) { _max_err = err; }

  /// set whether we should validate the direction using info from reco'd vertex
  void setValidateDirection(bool on) { _validate_dir = on; }

  /// set minimum dot product between shower direction and vtx -> strt direction
  void setMinDotProduct(double d) { _dot_min = d; }

  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

  // maximum error in the angle that is allowed
  double _max_err;

  // validate direction by using reconstructed vertex
  bool _validate_dir;

  // minimum dot product value
  double _dot_min;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

