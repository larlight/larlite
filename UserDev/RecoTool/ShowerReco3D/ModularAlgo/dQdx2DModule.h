/**
 * \file dQdx2DModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class dQdx2DModule
 *
 * @authors Joseph Zennamo, UChicago, jzennamo@uchicago.edu
 * @authors Brooke Russell, Yale U, brooke.russell@yale.edu
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
    //This variable will contain the average dQdx from the start of a shower projected into the 3 planes.
    //it is inteded to be filled such that element 1 contains the average dQdx for the U-plane, 
    //element 2 contains the same for the V-plane, and element 3 will be for the Y-plane. 
    std::vector<double> fdQdx; 

  };

} // showerreco

#endif
/** @} */ // end of doxygen group 

