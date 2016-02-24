/**
 * \file TrackEmuMaster.h
 *
 * \ingroup RecoEmulator
 *
 * \brief Master emulator that holds instances of other emulators and calls their Emulate function
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUMASTER_H__
#define __TRACKEMUMASTER_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"
#include "TrackEmuTrackFlip.h"
// #include "TrackEmuTrackDeviation.h"
// #include "TrackEmuApplyLengthEff.h"
#include "TrackEmuApplyTotalEff.h"
#include "TrackEmuTrackDeviationParametrized.h"

namespace recoemu {
  /**
     \class TrackEmuMaster
     User defined class TrackEmuMaster ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuMaster : public TrackEmulatorBase {

  public:

    /// Default constructor
    TrackEmuMaster();

    /// Default destructor
    ~TrackEmuMaster() {}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);

  private:

    TrackEmuTrackDeviationParametrized _trackdeviation;
    TrackEmuTrackFlip      _trackflip;
    TrackEmuApplyTotalEff _tracktotaleff;

  };

  /**
     \class larcv::TrackEmuMasterFactory
     \brief A concrete factory class for larcv::TrackEmuMaster
  */
  class TrackEmuMasterFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuMasterFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuMaster", this); }
    /// dtor
    ~TrackEmuMasterFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuMaster; }
  };
  /// Global recoemu::TrackEmuMasterFactory to register RecoEmulatorFactory
  static TrackEmuMasterFactory __global_TrackEmuMasterFactory__;
}

#endif
/** @} */ // end of doxygen group

