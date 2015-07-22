/**
 * \file dQdx2DModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class dQdx2DModule
 *
 * @authors Joseph Zennamo, UChicago, jzennamo@uchicago.edu
 * 
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef DQDX2DMODULE_H
#define DQDX2DMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
/**
   \class dQdx2DModule : ShowerRecoModuleBase
   This is meant to compute the 2D dQdx along the start of the shower. 
 */
namespace showerreco {

  class dQdx2DModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    dQdx2DModule(){_name = "dQdx2DModule";}

    /// Default destructor
    ~dQdx2DModule(){}


    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

  private:

    std::vector<double> fdQdx; 

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

