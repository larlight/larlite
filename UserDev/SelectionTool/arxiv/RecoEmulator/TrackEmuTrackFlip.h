/**
 * \file TrackEmuTrackFlip.h
 *
 * \ingroup RecoEmulator
 *
 * \brief Reads in a flat probability of flipping a track from a config file,
 * and uses this (plus a random draw) to decide if an emulated track should be flipped
 * (and by flip I mean the ordering of the trajectory points reversed)
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUTRACKFLIP_H__
#define __TRACKEMUTRACKFLIP_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"

namespace recoemu {
  /**
     \class TrackEmuTrackFlip
     User defined class TrackEmuTrackFlip ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuTrackFlip : public TrackEmulatorBase {

  public:

    /// Default constructor
    TrackEmuTrackFlip();

    /// Default destructor
    ~TrackEmuTrackFlip() {}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);

  private:

    double _flip_probability;
    
    RandomDrawTool _flipeff;

  };

  /**
     \class larcv::TrackEmuTrackFlipFactory
     \brief A concrete factory class for larcv::TrackEmuTrackFlip
  */
  class TrackEmuTrackFlipFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuTrackFlipFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuTrackFlip", this); }
    /// dtor
    ~TrackEmuTrackFlipFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuTrackFlip; }
  };
  /// Global recoemu::TrackEmuTrackFlipFactory to register RecoEmulatorFactory
  static TrackEmuTrackFlipFactory __global_TrackEmuTrackFlipFactory__;
}

#endif
/** @} */ // end of doxygen group

