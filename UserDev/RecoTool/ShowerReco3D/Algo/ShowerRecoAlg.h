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

#include "ShowerReco3D/Base/ShowerRecoAlgBase.h"
#include "ShowerReco3D/Base/ShowerCalo.h"
#include "LArUtil/GeometryHelper.h"
#include "AnalysisAlg/CalorimetryAlg.h"

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
    void Reset() { ShowerRecoAlgBase::Reset(); }

    /// Function to reconstruct a shower
    Shower_t RecoOneShower(const ProtoShower & );

    /// Function to decide if to use Area or Pulse Amplitude for calculations
    void SetUseArea(bool on) { fUseArea = on; }

    /// Function to decide if to use ModBox (True) or Birks (False) for calorimetry
    void SetUseModBox(bool on) { fCaloAlg->setUseModBox(on); }

    /// Function to set whether to use E correction
    void setEcorrection(bool on) { _Ecorrection = on; }

    /// Function to set if to use a linear scale to calculate Energy
    void SetUseLinearEnergy(bool on) { _linearE = on; }

    void CaloAlgo(::calo::CalorimetryAlg * c){fCaloAlg = c;}

  protected:

    larutil::GeometryHelper  *fGSer;
   
  private:

    /// Calorimetry algorithm
    ::calo::CalorimetryAlg *fCaloAlg;

    /// Boolean -> decide if to use energy correction or not
    bool _Ecorrection;

    /// use linear dQ -> dE energy conversion? If true over-rides
    /// either Box or Birks model
    bool _linearE;
    
    double fcalodEdxlength;
    double fdEdxlength;
    bool fUseArea;

    /// variables fot TTree
    double _dedx;
    int    _pl;
    std::vector<double> _dedx_v;

  };
}

#endif
/** @} */ // end of doxygen group 

