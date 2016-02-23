/**
 * \file ToyGeoModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class ToyGeoModule
 *
 * @author yuntse
 */

/** \addtogroup ModularAlgo

    @{*/

#ifndef TOYGEOMODULE_H
#define TOYGEOMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"

namespace showerreco {
  /**
     \class ToyGeoModule
     User custom analysis class made by SHELL_USER_NAME
   */
  class ToyGeoModule : ShowerRecoModuleBase {
  
  public:

    /// Default constructor
    ToyGeoModule(){ _name="ToyGeoModule"; }

    /// Default destructor
    ~ToyGeoModule(){}

    void do_reconstruction(const ProtoShower &, Shower_t &);

  private:

  };
}
#endif
/** @} */ // end of doxygen group 
