/**
 * \file ShowerEmuSmearing.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class ShowerEmuSmearing
 *
 * @author david caratelli
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __SHOWEREMUSMEARING_H__
#define __SHOWEREMUSMEARING_H__

#include "ShowerEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"

namespace recoemu {
  /**
     \class ShowerEmuSmearing
     User defined class ShowerEmuSmearing ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerEmuSmearing : public ShowerEmulatorBase{
    
  public:
    
    /// Default constructor
    ShowerEmuSmearing();
    
    /// Default destructor
    ~ShowerEmuSmearing(){}

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
    
  };

  /**
     \class larcv::ShowerEmuSmearingFactory
     \brief A concrete factory class for larcv::ShowerEmuSmearing
  */
  class ShowerEmuSmearingFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    ShowerEmuSmearingFactory() { RecoEmulatorFactory::get().add_factory("ShowerEmuSmearing",this); }
    /// dtor
    ~ShowerEmuSmearingFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new ShowerEmuSmearing; }
  };
  /// Global recoemu::ShowerEmuSmearingFactory to register RecoEmulatorFactory
  static ShowerEmuSmearingFactory __global_ShowerEmuSmearingFactory__;
}

#endif
/** @} */ // end of doxygen group 

