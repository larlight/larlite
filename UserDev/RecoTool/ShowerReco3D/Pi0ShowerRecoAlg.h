/**
 * \file Pi0ShowerRecoAlg.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class Pi0ShowerRecoAlg
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_PI0SHOWERRECOALG_H
#define RECOTOOL_PI0SHOWERRECOALG_H

#include "ShowerRecoAlgBase.h"
#include "AnalysisAlg/CalorimetryAlg.h"
namespace showerreco {
  
  /**
     \class Pi0ShowerRecoAlg
     User defined class Pi0ShowerRecoAlg ... these comments are used to generate
     doxygen documentation!
  */
  class Pi0ShowerRecoAlg : public ShowerRecoAlgBase{
    
  public:
    
    /// Default constructor
    Pi0ShowerRecoAlg();
    
    /// Default destructor
    virtual ~Pi0ShowerRecoAlg(){}

    /// Function to reset algorithm, to be called @ beginning of each event
    virtual void Reset(){ ShowerRecoAlgBase::Reset(); }

    /// Function to reconstruct a shower
    virtual ::larlite::shower RecoOneShower(const std::vector< ::showerreco::ShowerCluster_t>& );

  protected:

    larutil::GeometryUtilities  *fGSer;
   
  private:
    
   double fcalodEdxlength;
   double fdEdxlength;
   bool fUseArea;
   
   
  };
}

#endif
/** @} */ // end of doxygen group 

