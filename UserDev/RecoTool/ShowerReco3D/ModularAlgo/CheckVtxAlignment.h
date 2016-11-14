/**
 * \file CheckVtxAlignment.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class CheckVtxAlignment
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef CHECKVTXALIGNMENT_H
#define CHECKVTXALIGNMENT_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class CheckVtxAlignment : ShowerRecoModuleBase {

public:

  /// Default constructor
  CheckVtxAlignment() {_name = "CheckVtxAlignment";}

  /// Default destructor
  ~CheckVtxAlignment() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

