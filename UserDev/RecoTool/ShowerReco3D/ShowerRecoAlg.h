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

  protected:

    /// Function to reconstruct a shower
    virtual ::larlite::shower RecoOneShower(const std::vector< ::showerreco::ShowerCluster_t>& );

  protected:

   ::calo::CalorimetryAlg fCaloAlg;

    larutil::GeometryUtilities  *fGSer;
   
  private:
    
   double fcalodEdxlength;
   double fdEdxlength;
   bool fUseArea;
   
   
  };
}

#endif
/** @} */ // end of doxygen group 

