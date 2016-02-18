/**
 * \file TrackEmuApplyLengthEff.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Class uses the following function:  ((atan([1]*(x-[0]))/pi) + 0.5)*[2]
 * where "x" is track length in cm, the return value is the efficiency to reconstruct that track,
 * and [0],[1],[2] are settable parameters in a config file.
 * This emulator will potentially delete the track based on the efficiency.
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUAPPLYLENGTHEFF_H__
#define __TRACKEMUAPPLYLENGTHEFF_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"

namespace recoemu {
  /**
     \class TrackEmuApplyLengthEff
     User defined class TrackEmuApplyLengthEff ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuApplyLengthEff : public TrackEmulatorBase{
    
  public:
    
    /// Default constructor
    TrackEmuApplyLengthEff();
    
    /// Default destructor
    ~TrackEmuApplyLengthEff(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);

  private:
    
    RandomDrawTool _lengthEff;
    
    double LengthEffParam0;
    double LengthEffParam1;
    double LengthEffParam2;

  };

  /**
     \class larcv::TrackEmuApplyLengthEffFactory
     \brief A concrete factory class for larcv::TrackEmuApplyLengthEff
  */
  class TrackEmuApplyLengthEffFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuApplyLengthEffFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuApplyLengthEff",this); }
    /// dtor
    ~TrackEmuApplyLengthEffFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuApplyLengthEff; }
  };
  /// Global recoemu::TrackEmuApplyLengthEffFactory to register RecoEmulatorFactory
  static TrackEmuApplyLengthEffFactory __global_TrackEmuApplyLengthEffFactory__;
}

#endif
/** @} */ // end of doxygen group 

