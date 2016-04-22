/**
 * \file ShowerEmuSmearingRecoBased.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class ShowerEmuSmearingRecoBased
 *
 * @author david caratelli
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __SHOWEREMUSMEARINGRECOBASED_H__
#define __SHOWEREMUSMEARINGRECOBASED_H__

#include "ShowerEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"

namespace recoemu {
  /**
     \class ShowerEmuSmearingRecoBased
     User defined class ShowerEmuSmearingRecoBased ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerEmuSmearingRecoBased : public ShowerEmulatorBase{
    
  public:
    
    /// Default constructor
    ShowerEmuSmearingRecoBased();
    
    /// Default destructor
    ~ShowerEmuSmearingRecoBased(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Shower_t Emulate(const recoemu::Shower_t&);

  private:
    
    // smearing functions
    RandomDrawTool _energySmear;
    RandomDrawTool _angleSmear;
    RandomDrawTool _xSmear;
    RandomDrawTool _ySmear;
    RandomDrawTool _zSmear;
    RandomDrawTool _dedxSmear;

    // offset from central value of dEdx (assumed 2 & 4 for e-/gamma)
    double _dEdxResolutionOffset;
    
  };

  /**
     \class larcv::ShowerEmuSmearingRecoBasedFactory
     \brief A concrete factory class for larcv::ShowerEmuSmearingRecoBased
  */
  class ShowerEmuSmearingRecoBasedFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    ShowerEmuSmearingRecoBasedFactory() { RecoEmulatorFactory::get().add_factory("ShowerEmuSmearingRecoBased",this); }
    /// dtor
    ~ShowerEmuSmearingRecoBasedFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new ShowerEmuSmearingRecoBased; }
  };
  /// Global recoemu::ShowerEmuSmearingRecoBasedFactory to register RecoEmulatorFactory
  static ShowerEmuSmearingRecoBasedFactory __global_ShowerEmuSmearingRecoBasedFactory__;
}

#endif
/** @} */ // end of doxygen group 

