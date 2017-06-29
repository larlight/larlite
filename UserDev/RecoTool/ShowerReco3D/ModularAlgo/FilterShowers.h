/**
 * \file FilterShowers.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class FilterShowers
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef FILTERSHOWERS_H
#define FILTERSHOWERS_H

#include <iostream>
#include "ShowerRecoModuleBase.h"

#include <sstream>

/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class FilterShowers : ShowerRecoModuleBase {

public:

  /// Default constructor
  FilterShowers() {_name = "FilterShowers";}

  /// Default destructor
  ~FilterShowers() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

  void setAngleCut(double a) { _anglecut = a; }

private:

  double _anglecut;

};

} // showerreco

#endif
/** @} */ // end of doxygen group

