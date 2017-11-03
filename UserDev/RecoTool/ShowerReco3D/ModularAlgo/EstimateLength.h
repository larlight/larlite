/**
 * \file EstimateLength.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class EstimateLength
 *
 * @author noone
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef ESTIMATELENGTH_H
#define ESTIMATELENGTH_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class EstimateLength : ShowerRecoModuleBase {

public:

  /// Default constructor
  EstimateLength();

  /// Default destructor
  ~EstimateLength() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

