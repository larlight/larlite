/**
 * \file TrackEmuTrackDeviationParametrized.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class def header for a class TrackEmuTrackDeviationParamaterized
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUTRACKDEVIATIONPARAMETRIZED_H__
#define __TRACKEMUTRACKDEVIATIONPARAMETRIZED_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "TF1.h"

namespace recoemu {
  /**
     \class TrackEmuTrackDeviationParametrized
     User defined class TrackEmuTrackDeviationParametrized ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuTrackDeviationParametrized : public TrackEmulatorBase{
    
  public:
    
    /// Default constructor
    TrackEmuTrackDeviationParametrized();
    
    /// Default destructor
    ~TrackEmuTrackDeviationParametrized(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);

  private:

    // TF1s
    TF1* _fXSmear;
    TF1* _fYSmear;
    TF1* _fZSmear;

  };

  /**
     \class larcv::TrackEmuSmearingFactory
     \brief A concrete factory class for larcv::TrackEmuSmearing
  */
  class TrackEmuTrackDeviationParametrizedFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuTrackDeviationParametrizedFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuTrackDeviationParametrized",this); }
    /// dtor
    ~TrackEmuTrackDeviationParametrizedFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuTrackDeviationParametrized; }
  };
  /// Global recoemu::TrackEmuSmearingFactory to register RecoEmulatorFactory
  static TrackEmuTrackDeviationParametrizedFactory __global_TrackEmuTrackDeviationParametrizedFactory__;
}

#endif
/** @} */ // end of doxygen group 

