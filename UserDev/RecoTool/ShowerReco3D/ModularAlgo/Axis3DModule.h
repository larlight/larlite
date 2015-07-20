/**
 * \file Axis3DModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class Axis3DModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef AXIS3DMODULE_H
#define AXIS3DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class Axis3DModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    Axis3DModule(){_name = "Axis3DModule";}

    /// Default destructor
    ~Axis3DModule(){}


    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  private:

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

