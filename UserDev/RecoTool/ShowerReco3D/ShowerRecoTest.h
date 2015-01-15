/**
 * \file ShowerRecoTest.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerRecoTest
 *
 * @author David Caratelli
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_SHOWERRECOTEST_H
#define RECOTOOL_SHOWERRECOTEST_H

#include "ShowerRecoAlgBase.h"
#include "AnalysisAlg/CalorimetryAlg.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ShowerCalo.h"

namespace showerreco {
  
  /**
     \class ShowerRecoTest
     User defined class ShowerRecoTest ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoTest : public ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    ShowerRecoTest();
    
    /// Default destructor
    virtual ~ShowerRecoTest(){}

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

    /// Include instance of GeoAlgo
    geoalgo::GeoAlgo _geoAlgo;

    /// Boolean -> decide if to use energy correction or not
    bool _Ecorrection;
    
    double fcalodEdxlength;
    double fdEdxlength;
    bool fUseArea;

  };
}

#endif
/** @} */ // end of doxygen group 

