/**
 * \file DetectorProperties.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class DetectorProperties
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARLITE_DETECTORPROPERTIES_H
#define LARLITE_DETECTORPROPERTIES_H

#include "LArUtilBase.h"

namespace larutil {
  /**
     \class DetectorProperties
     LArSoft's DetectorProperties service. Database part is currently not implemented.
     Also we ignore the idea of "inherit properties from history": as that does not seem to be
     a convincing solution yet adding quite a bit of complexity.
     Once we have DB, Kazu will implement a simple DBI one can use from command line.
  */
  class DetectorProperties : public LArUtilBase {

  private:

    /// Singleton constructor
    DetectorProperties(bool default_load=true);

    /// Default destructor
    virtual ~DetectorProperties(){};

    static DetectorProperties* _me;
    
  public:

    /// Singleton getter
    static const DetectorProperties* GetME(bool default_load=true)
    {
      if(!_me) _me = new DetectorProperties(default_load);
      return _me;
    }

    //--- LArSoft Function Implementations ---//
    
    Double_t SamplingRate()      const { return fSamplingRate;   }
    Double_t ElectronsToADC()    const { return fElectronsToADC; }
    UInt_t   NumberTimeSamples() const { return fNumberTimeSamples; }
    UInt_t   ReadOutWindowSize() const { return fReadOutWindowSize; }
    Int_t    TriggerOffset()     const { return fTriggerOffset;}
    Double_t TimeOffsetU()       const { return fTimeOffsetU; }
    Double_t TimeOffsetV()       const { return fTimeOffsetV; }
    Double_t TimeOffsetZ()       const { return fTimeOffsetZ; }
    
    Double_t ConvertXToTicks(Double_t X,     Int_t p) const {
      return (X / fXTicksCoefficient +  fXTicksOffsets.at(p));
    }

    Double_t ConvertTicksToX(Double_t ticks, Int_t p) const {
      return (ticks - fXTicksOffsets.at(p) ) * fXTicksCoefficient;
    }
    
    Double_t GetXTicksOffset(Int_t p) const {
      return fXTicksOffsets.at(p);
    }

    Double_t GetXTicksCoefficient() const {
      return fXTicksCoefficient;
    }
    
    // Following functions are intentionally commented out by Kazu for LArLite
    // as they do not make any sense to him (he thinks they are wrong).
    /*
    Double_t             ConvertTDCToTicks(Double_t tdc) const 
    { return fNumberTimeSamples == fReadOutWindowSize ? tdc : tdc - fReadOutWindowSize;}

    Double_t             ConvertTicksToTDC(Double_t ticks) const
    { return fNumberTimeSamples == fReadOutWindowSize ? ticks : ticks + fReadOutWindowSize;}
    */    

  protected:

    virtual bool ReadTree();
    virtual void ClearData();

    Double_t fSamplingRate;      ///< in ns
    Int_t    fTriggerOffset;     ///< in # of clock ticks
    Double_t fElectronsToADC;    ///< conversion factor for # of ionization electrons to 1 ADC count
    UInt_t   fNumberTimeSamples; ///< number of clock ticks per event
    UInt_t   fReadOutWindowSize; ///< number of clock ticks per readout window
    Double_t fTimeOffsetU;       ///< time offsets to convert spacepoint
    Double_t fTimeOffsetV;       ///< coordinates to hit times on each
    Double_t fTimeOffsetZ;       ///< view
    
    bool     fInheritTriggerOffset;     ///< Flag saying whether to inherit TriggerOffset
    bool     fInheritNumberTimeSamples; ///< Flag saying whether to inherit NumberTimeSamples
    
    Double_t fXTicksCoefficient; ///< Parameters for x<-->ticks
    bool     fXTicksParamsLoaded;///<  calculations
    std::vector<Double_t> fXTicksOffsets;

  };
}
#endif
/** @} */ // end of doxygen group 

