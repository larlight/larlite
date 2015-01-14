/**
 * \file ShowerRecoAlg.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerRecoAlg
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_SHOWERRECOALG_H
#define RECOTOOL_SHOWERRECOALG_H

#include "ShowerRecoAlgBase.h"
#include "AnalysisAlg/CalorimetryAlg.h"
#include "ShowerCalo.h"

namespace showerreco {
  
  /**
     \class ShowerRecoAlg
     User defined class ShowerRecoAlg ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoAlg : public ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    ShowerRecoAlg();
    
    /// Default destructor
    virtual ~ShowerRecoAlg(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset() { ShowerRecoAlgBase::Reset(); }

    /// Function to decide if to use Area or Pulse Amplitude for calculations
    void SetUseArea(bool on) { fUseArea = on; }

    /// Function to decide if to use ModBox (True) or Birks (False) for calorimetry
    void SetUseModBox(bool on) { fCaloAlg.setUseModBox(on); }

    /// Function to set whether to use E correction
    void setEcorrection(bool on) { _Ecorrection = on; }

  protected:

    /// Function to reconstruct a shower
    virtual ::larlite::shower RecoOneShower(const std::vector< ::showerreco::ShowerCluster_t>& );

  protected:

   ::calo::CalorimetryAlg fCaloAlg;

    larutil::GeometryUtilities  *fGSer;
   
  private:

    /// Boolean -> decide if to use energy correction or not
    bool _Ecorrection;
    
    double fcalodEdxlength;
    double fdEdxlength;
    bool fUseArea;

  };
}

#endif
/** @} */ // end of doxygen group 

