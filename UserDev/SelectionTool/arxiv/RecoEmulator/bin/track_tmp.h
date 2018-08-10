/**
 * \file TrackEmuClassName.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class TrackEmuClassName
 *
 * @author USER
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUCLASSNAME_H__
#define __TRACKEMUCLASSNAME_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"

namespace recoemu {
  /**
     \class TrackEmuClassName
     User defined class TrackEmuClassName ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuClassName : public TrackEmulatorBase{
    
  public:
    
    /// Default constructor
    TrackEmuClassName() : TrackEmulatorBase("TrackEmuClassName") {}
    
    /// Default destructor
    ~TrackEmuClassName(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);
    
  };

  /**
     \class larcv::TrackEmuClassNameFactory
     \brief A concrete factory class for larcv::TrackEmuClassName
  */
  class TrackEmuClassNameFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuClassNameFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuClassName",this); }
    /// dtor
    ~TrackEmuClassNameFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuClassName; }
  };
  /// Global recoemu::TrackEmuClassNameFactory to register RecoEmulatorFactory
  static TrackEmuClassNameFactory __global_TrackEmuClassNameFactory__;
}

#endif
/** @} */ // end of doxygen group 

