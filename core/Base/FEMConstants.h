#ifndef LARLITE_FEMCONSTANTS_H
#define LARLITE_FEMCONSTANTS_H

#include "DataFormatConstants.h"

namespace larlite {
  namespace fem {

    const UShort_t kINVALID_CH=data::kINVALID_USHORT; ///< Invalid (or initialized) channel number
    const UInt_t   kINVALID_WORD=data::kINVALID_UINT; ///< Invalid (or initialized) event word
    const UInt_t   kEMPTY_WORD=0x0;                  ///< Empty word
    const UInt_t   kBEAMGATE_NSAMPLE=1500;  ///< # samples in the beamgate
    const UShort_t kNUM_PMT_CHANNEL=48;     ///< # of PMT readout channels
    const UShort_t kMAX_ADC=4095;           ///< Maximum ADC value

    /// PMT discriminator type
    enum discriminator {
      kBaseDisc = 0, ///< lowest disc. threshold so-called disc. 0
      kCosmicDisc,   ///< discriminator for cosmics
      kMichelDisc,   ///< discriminator for michel electrons (not used, just a place hlder)
      kBeamDisc,     ///< discriminator for signal within the beam window
      kBeamWin,      ///< discriminator for beam window (forced N samples read-out)
      kDiscMax
    };
    /// Defines PMT word-type
    enum fem_word{
      kUndefinedWord = 0,    ///< Word type ... undefined
      kEventHeader,          ///< A signal word (no data) to flag the start of headers
      kFemHeader,            ///< Event header word
      kFemFirstWord,         ///< First word in the event (after the event header, before channel data )
      kChannelHeader,        ///< Channel header word
      kChannelWord,          ///< Channel word such as ADC sample
      kChannelLastWord,      ///< Last word per channel-wise readout
      kFemLastWord,          ///< Last word per fem
      kEventLastWord,        ///< Last word in the event
      kChannelTime,          ///< Channel time word
      kChannelPacketLastWord ///< Channel: last word of the packet
    };
  }

}

#endif
