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

  class ShowerChargeModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    ShowerChargeModule(){_name = "ShowerChargeModule";}

    /// Default destructor
    ~ShowerChargeModule(){}

    std::string name(){return _name;}

    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  private:
    std::string _name;

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

