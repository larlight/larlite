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
    const UInt_t   kINVALID_WORD=data::kINVALID_UINT; ///< Invalid (or initialized) event word
    const UInt_t   kEMPTY_WORD=0x0;                  ///< Empty word
    
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

        /// Defines PMT word-type
    enum FEM_WORD{
      kUNDEFINED_WORD = 0, ///< Word type ... undefined
      kEVENT_HEADER,       ///< A signal word (no data) to flag the start of headers 
      kFEM_HEADER,         ///< Event header word
      kFEM_FIRST_WORD,     ///< First word in the event (after the event header, before channel data )
      kCHANNEL_HEADER,     ///< Channel header word
      kCHANNEL_WORD,       ///< Channel word such as ADC sample
      kCHANNEL_LAST_WORD,  ///< Last word per channel-wise readout
      kFEM_LAST_WORD,      ///< Last word per fem
      kEVENT_LAST_WORD,    ///< Last word in the event
      kCHANNEL_TIME,       ///< Channel time word
      kCHANNEL_PACKET_LAST_WORD  ///< Channel: last word of the packet
    };

  }


    // namespace FEM{
    
    // const UShort_t INVALID_CH=data::kINVALID_USHORT; ///< Invalid (or initialized) channel number
    // const UInt_t   INVALID_WORD=data::kINVALID_UINT; ///< Invalid (or initialized) event word
    // const UInt_t   EMPTY_WORD=0x0;                  ///< Empty word
    
    // const UInt_t   BEAMGATE_NSAMPLE=1500;  ///< # samples in the beamgate
    // const UShort_t NUM_PMT_CHANNEL=48;     ///< # of PMT readout channels
    // const UShort_t MAX_ADC=4095;           ///< Maximum ADC value
    
    // /// PMT discriminator type
    // enum DISCRIMINATOR {
    //   BASE_DISC = 0, ///< lowest disc. threshold so-called disc. 0
    //   COSMIC_DISC,   ///< discriminator for cosmics
    //   MICHEL_DISC,   ///< discriminator for michel electrons (not used, just a place hlder)
    //   BEAM_DISC,     ///< discriminator for signal within the beam window
    //   BEAM_WIN,      ///< discriminator for beam window (forced N samples read-out)
    //   DISC_MAX
    // };

    // /// Defines PMT word-type
    // enum FEM_WORD{
    //   UNDEFINED_WORD = 0, ///< Word type ... undefined
    //   EVENT_HEADER,       ///< A signal word (no data) to flag the start of headers 
    //   FEM_HEADER,         ///< Event header word
    //   FEM_FIRST_WORD,     ///< First word in the event (after the event header, before channel data )
    //   CHANNEL_HEADER,     ///< Channel header word
    //   CHANNEL_WORD,       ///< Channel word such as ADC sample
    //   CHANNEL_LAST_WORD,  ///< Last word per channel-wise readout
    //   FEM_LAST_WORD,      ///< Last word per fem
    //   EVENT_LAST_WORD,    ///< Last word in the event
    //   CHANNEL_TIME,       ///< Channel time word
    //   CHANNEL_PACKET_LAST_WORD  ///< Channel: last word of the packet
    // };



}
#endif
/** @} */ // end of doxygen group
