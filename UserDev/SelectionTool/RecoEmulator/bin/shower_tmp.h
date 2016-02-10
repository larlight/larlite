/**
 * \file ShowerEmuClassName.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class ShowerEmuClassName
 *
 * @author USER
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __SHOWEREMUCLASSNAME_H__
#define __SHOWEREMUCLASSNAME_H__

#include "ShowerEmulatorBase.h"
#include "RecoEmulatorFactory.h"

namespace recoemu {
  /**
     \class ShowerEmuClassName
     User defined class ShowerEmuClassName ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerEmuClassName : public ShowerEmulatorBase{
    
  public:
    
    /// Default constructor
    ShowerEmuClassName() : ShowerEmulatorBase("ShowerEmuClassName") {}
    
    /// Default destructor
    ~ShowerEmuClassName(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Shower_t Emulate(const recoemu::Shower_t&);
    
  };

  /**
     \class larcv::ShowerEmuClassNameFactory
     \brief A concrete factory class for larcv::ShowerEmuClassName
  */
  class ShowerEmuClassNameFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    ShowerEmuClassNameFactory() { RecoEmulatorFactory::get().add_factory("ShowerEmuClassName",this); }
    /// dtor
    ~ShowerEmuClassNameFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new ShowerEmuClassName; }
  };
  /// Global recoemu::ShowerEmuClassNameFactory to register RecoEmulatorFactory
  static ShowerEmuClassNameFactory __global_ShowerEmuClassNameFactory__;
}

#endif
/** @} */ // end of doxygen group 

