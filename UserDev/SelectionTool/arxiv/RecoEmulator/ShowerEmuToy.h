/**
 * \file ShowerEmuToy.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class ShowerEmuToy
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __SHOWEREMUTOY_H__
#define __SHOWEREMUTOY_H__

#include "ShowerEmulatorBase.h"
#include "RecoEmulatorFactory.h"

namespace recoemu {
  /**
     \class ShowerEmuToy
     \brief Toy shower reco emulation algorithm. It emulates nothing.
  */
  class ShowerEmuToy : public ShowerEmulatorBase{
    
  public:
    
    /// Default constructor
    ShowerEmuToy() : ShowerEmulatorBase("ShowerEmuToy") {}
    
    /// Default destructor
    ~ShowerEmuToy(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator method implementation
    recoemu::Shower_t Emulate(const recoemu::Shower_t&);
    
  };

  /**
     \class larcv::ShowerEmuToyFactory
     \brief A concrete factory class for larcv::ShowerEmuToy
  */
  class ShowerEmuToyFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    ShowerEmuToyFactory() { RecoEmulatorFactory::get().add_factory("ShowerEmuToy",this); }
    /// dtor
    ~ShowerEmuToyFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new ShowerEmuToy; }
  };
  /// Global recoemu::ShowerEmuToyFactory to register RecoEmulatorFactory
  static ShowerEmuToyFactory __global_ShowerEmuToyFactory__;
}

#endif
/** @} */ // end of doxygen group 

