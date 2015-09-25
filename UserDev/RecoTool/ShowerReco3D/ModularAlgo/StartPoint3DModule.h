/**
 * \file StartPoint3DModule.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class StartPoint3DModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef STARTPOINT3DMODULE_H
#define STARTPOINT3DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class StartPoint3DModule : ShowerRecoModuleBase {

public:

  /// Default constructor
  StartPoint3DModule() {_name = "StartPoint3DModule"; }

  /// Default destructor
  ~StartPoint3DModule() {}

  /// Inherited/overloaded function from ShowerRecoModuleBase
  void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

};

} // showerreco

#endif
/** @} */ // end of doxygen group

