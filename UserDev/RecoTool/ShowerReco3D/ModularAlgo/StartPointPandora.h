/**
 * \file StartPointPandora.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class StartPointPandora
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef STARTPOINTPANDORAMODULE_H
#define STARTPOINTPANDORAMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class StartPointPandora : ShowerRecoModuleBase {

public:

  /// Default constructor
  StartPointPandora() {_name = "StartPointPandora";}

  /// Default destructor
  ~StartPointPandora() {}


  void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

private:

};

} // showerreco

#endif
/** @} */ // end of doxygen group

