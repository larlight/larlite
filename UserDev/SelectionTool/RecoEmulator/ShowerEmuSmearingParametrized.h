/**
 * \file ShowerEmuSmearingRecoBased.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class ShowerEmuSmearingParametrized
 *
 * @author david caratelli
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __SHOWEREMUSMEARINGPARAMETRIZED_H__
#define __SHOWEREMUSMEARINGPARAMETRIZED_H__

#include "ShowerEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "TF1.h"

namespace recoemu {
  /**
     \class ShowerEmuSmearingParametrized
     User defined class ShowerEmuSmearingParametrized ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerEmuSmearingParametrized : public ShowerEmulatorBase{
    
  public:
    
    /// Default constructor
    ShowerEmuSmearingParametrized();
    
    /// Default destructor
    ~ShowerEmuSmearingParametrized(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Shower_t Emulate(const recoemu::Shower_t&);

  private:
    
    TF1* _fEres;
    TF1* _fAngleres;
    TF1* _fdEdxres;
    TF1* _fPosres;
  
  };

  /**
     \class larcv::ShowerEmuSmearingFactory
     \brief A concrete factory class for larcv::ShowerEmuSmearing
  */
  class ShowerEmuSmearingParametrizedFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    ShowerEmuSmearingParametrizedFactory() { RecoEmulatorFactory::get().add_factory("ShowerEmuSmearingParametrized",this); }
    /// dtor
    ~ShowerEmuSmearingParametrizedFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new ShowerEmuSmearingParametrized; }
  };
  /// Global recoemu::ShowerEmuSmearingFactory to register RecoEmulatorFactory
  static ShowerEmuSmearingParametrizedFactory __global_ShowerEmuSmearingParametrizedFactory__;
}

#endif
/** @} */ // end of doxygen group 

