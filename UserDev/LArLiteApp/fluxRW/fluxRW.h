
/*Code that reweighs MCC5 generated BNB MicroBooNE events according to 
MiniBooNE fluxes (which are currently thought to be more correct) based on 
neutrino energy, type and parentage.

Based on FluxRW found at: 
https://github.com/larlight/LArLight/tree/lar1/UserDev/lar1Osc/

Created by Robert Murrells:
robertmurrells@gmail.com
*/

/**
 * \file fluxRW.h
 *
 * \ingroup sbnrec
 * 
 * \brief Class def header for a class fluxRW
 *
 * @author murrells
 */

/** \addtogroup sbnrec

    @{*/

#ifndef FLUXRW_H
#define FLUXRW_H

#include <iostream>
#include "Rtypes.h"
#include "DataFormat/storage_manager.h"

/**
   \class fluxRW
   User defined class fluxRW ... these comments are used to generate
   doxygen documentation!
 */



  class fluxRW{
    
  public:
    
    /// Default constructor
    fluxRW();
    
    /// Default destructor
    ~fluxRW(){}
    
    Double_t get_weight(Double_t energy, Int_t ntype, Int_t ptype);
    
    Double_t event_weight(larlite::storage_manager * storage);
    
  private:
    
    Double_t bin_size;
    
    double fCV[4][4][200];
    double fRW[4][4][200];

    
  };

#endif
/** @} */ // end of doxygen group 

