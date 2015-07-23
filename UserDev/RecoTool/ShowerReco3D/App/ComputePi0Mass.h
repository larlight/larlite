/**
 * \file ComputePi0Mass.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief This class is to run matching on pi0 files and calculate a pi0 mass peak from their output.
 *
 * @author davidkaleko
 */

/** \addtogroup ShowerReco3D

    @{*/

#ifndef RECOTOOL_COMPUTEPI0MASS_H
#define RECOTOOL_COMPUTEPI0MASS_H

#include "Analysis/ana_base.h"
#include "MCComp/MCMatchAlg.h"
#include "ShowerReco3D/Base/ShowerCalo.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include <TH1D.h>
namespace larlite {
  /**
     \class ComputePi0Mass
     User custom analysis class made by davidkaleko
   */
  class ComputePi0Mass : public ana_base{
  
  public:

    /// Default constructor
    ComputePi0Mass(){ 
      _name="ComputePi0Mass"; 
      _fout=0; 
      _debug=false; 
      _shower_producer="";
    };

    /// Default destructor
    virtual ~ComputePi0Mass(){};

    /** IMPLEMENT in ComputePi0Mass.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ComputePi0Mass.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ComputePi0Mass.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetShowerProducer(const std::string name) { _shower_producer = name; }

    void SetDebug(bool flag) { _debug = flag; }

    float Pi0MassFormula3D(  float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2);

    static ComputePi0Mass* GetInstance() { 
      if(!me) me = new ComputePi0Mass;
      return me; 
    }
    
    float GetMass(){ return _mass; };

  protected:

    ::btutil::MCMatchAlg fBTAlg;

    ::showerreco::ShowerCalo fSECaloAlg;

    std::string _shower_producer;
    
    TH1D* hPi0MassPeak;
    TH1D* hDaughterCorrectness;
    TH1D* hDaughterVtxDist;
    TH1D* hDaughterAngleDiff;

    bool _debug;
    
    float _mass;

  private:
    
    static ComputePi0Mass *me; // attempt at a shared object instance ptr

  };
}
#endif

/** @} */ // end of doxygen group 
