/**
 * \file TrackEmuToy.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class TrackEmuToy
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUTOY_H__
#define __TRACKEMUTOY_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"

namespace recoemu {
  /**
     \class TrackEmuToy
     \brief Toy track reco emulation algorithm. It emulates nothing.
  */
  class TrackEmuToy : public TrackEmulatorBase{
    
  public:
    
    /// Default constructor
    TrackEmuToy() : TrackEmulatorBase("TrackEmuToy") {}
    
    /// Default destructor
    ~TrackEmuToy(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);
    
  };

  /**
     \class larcv::TrackEmuToyFactory
     \brief A concrete factory class for larcv::TrackEmuToy
  */
  class TrackEmuToyFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuToyFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuToy",this); }
    /// dtor
    ~TrackEmuToyFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuToy; }
  };
  /// Global recoemu::TrackEmuToyFactory to register RecoEmulatorFactory
  static TrackEmuToyFactory __global_TrackEmuToyFactory__;
}

#endif
/** @} */ // end of doxygen group 

