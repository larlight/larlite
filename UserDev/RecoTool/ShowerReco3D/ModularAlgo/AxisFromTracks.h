/**
 * \file AxisFromTracks.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class AxisFromTracks
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef AXISFROMTRACKS_H
#define AXISFROMTRACKS_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class AxisFromTracks : ShowerRecoModuleBase {

public:

  /// Default constructor
  AxisFromTracks() {_name = "AxisFromTracks";}

  /// Default destructor
  ~AxisFromTracks() {}


  void do_reconstruction(const ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

