/**
 * \file FEMConstants.h
 *
 * \ingroup Base
 * 
 * \brief defines FEM electronics related constants
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_FEMCONSTANTS_H
#define LARLITE_FEMCONSTANTS_H
#include "DataFormatConstants.h"

/// Namespace of everything in this framework
namespace larlite{
  
  /// Defines constants for PMT related electronics
  namespace fem{
    
    const UShort_t kINVALID_CH=data::kINVALID_USHORT; ///< Invalid (or initialized) channel number
    
    const UInt_t   kBEAMGATE_NSAMPLE=1500;  ///< # samples in the beamgate
    const UShort_t kNUM_PMT_CHANNEL=48;     ///< # of PMT readout channels
    const UShort_t kMAX_ADC=4095;           ///< Maximum ADC value
    
    /// PMT discriminator type
    enum Discriminator_t{
      kBASE_DISC = 0, ///< lowest disc. threshold so-called disc. 0
      kCOSMIC_DISC,   ///< discriminator for cosmics
      kMICHEL_DISC,   ///< discriminator for michel electrons (not used, just a place hlder)
      kBEAM_DISC,     ///< discriminator for signal within the beam window
      kBEAM_WIN,      ///< discriminator for beam window (forced N samples read-out)
      kDISC_MAX
    };
  }
}
#endif
/** @} */ // end of doxygen group
