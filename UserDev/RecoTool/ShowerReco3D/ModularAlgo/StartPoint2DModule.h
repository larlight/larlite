/**
 * \file StartPoint2DModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class StartPoint2DModule
 *
 * @author cadams
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef STARTPOINT2DMODULE_H
#define STARTPOINT2DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class StartPoint2DModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    StartPoint2DModule(){_name = "StartPoint2DModule";}

    /// Default destructor
    ~StartPoint2DModule(){}

    std::string name(){return _name;}

    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  private:
    std::string _name;

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

