/**
 * \file ShowerChargeModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class ShowerChargeModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef SHOWERCHARGEMODULE_H
#define SHOWERCHARGEMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class ShowerChargeModule : ShowerRecoModuleBase {

public:

  /// Default constructor
  ShowerChargeModule() {_name = "ShowerChargeModule";}

  /// Default destructor
  ~ShowerChargeModule() {}


  void do_reconstruction(const ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

