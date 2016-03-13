/**
 * \file TrackEmuTrackDeviation.h
 *
 * \ingroup RecoEmulator
 *
 * \brief Loops through points on a track and smears them in the x-, y-, and z-
 * directions according to different input (cfg file) histogram distributions.
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUTRACKDEVIATION_H__
#define __TRACKEMUTRACKDEVIATION_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"

namespace recoemu {
  /**
     \class TrackEmuTrackDeviation
     User defined class TrackEmuTrackDeviation ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuTrackDeviation : public TrackEmulatorBase {

  public:

    /// Default constructor
    TrackEmuTrackDeviation();

    /// Default destructor
    ~TrackEmuTrackDeviation() {}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);

  private:

    RandomDrawTool _xres;
    RandomDrawTool _yres;
    RandomDrawTool _zres;

  };

  /**
     \class larcv::TrackEmuTrackDeviationFactory
     \brief A concrete factory class for larcv::TrackEmuTrackDeviation
  */
  class TrackEmuTrackDeviationFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuTrackDeviationFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuTrackDeviation", this); }
    /// dtor
    ~TrackEmuTrackDeviationFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuTrackDeviation; }
  };
  /// Global recoemu::TrackEmuTrackDeviationFactory to register RecoEmulatorFactory
  static TrackEmuTrackDeviationFactory __global_TrackEmuTrackDeviationFactory__;
}

#endif
/** @} */ // end of doxygen group

