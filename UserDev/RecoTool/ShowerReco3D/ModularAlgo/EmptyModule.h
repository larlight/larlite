/**
 * \file EmptyModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class EmptyModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef EMPTYMODULE_H
#define EMPTYMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class EmptyModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    EmptyModule(){_name = "EmptyModule";}

    /// Default destructor
    ~EmptyModule(){}

    std::string name(){return _name;}

    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  private:
    std::string _name;

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

