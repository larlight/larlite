/**
 * \file ShowerRecoModuleBase.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ShowerRecoModuleBase
 *
 * @author cadams
 */

/** \addtogroup Algo

    @{*/
#ifndef SHOWERRECOMODULEBASE_H
#define SHOWERRECOMODULEBASE_H

#include <iostream>
#include "ShowerReco3D/Base/ShowerRecoTypes.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class ShowerRecoModuleBase{

  public:

    /// Default constructor
    ShowerRecoModuleBase(){}

    /// Default destructor
    virtual ~ShowerRecoModuleBase(){}

    std::string name(){return _name;}

    virtual void do_reconstruction(const ShowerClusterSet_t &, Shower_t &) = 0;

  private:
    std::string _name;

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

