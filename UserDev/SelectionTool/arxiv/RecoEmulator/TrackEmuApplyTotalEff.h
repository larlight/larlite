/**
 * \file TrackEmuApplyTotalEff.h
 *
 * \ingroup RecoEmulator
 * 
 * \brief Computes length efficiency, 2D angle efficiency, and applies them both multiplied.
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef __TRACKEMUAPPLYTOTALEFF_H__
#define __TRACKEMUAPPLYTOTALEFF_H__

#include "TrackEmulatorBase.h"
#include "RecoEmulatorFactory.h"
#include "RandomDrawTool.h"

namespace recoemu {
  /**
     \class TrackEmuApplyTotalEff
     User defined class TrackEmuApplyTotalEff ... these comments are used to generate
     doxygen documentation!
  */
  class TrackEmuApplyTotalEff : public TrackEmulatorBase{
    
  public:
    
    /// Default constructor
    TrackEmuApplyTotalEff();
    
    /// Default destructor
    ~TrackEmuApplyTotalEff(){}

    /// Configuration method
    void Configure(const ::fcllite::PSet&);

    /// Emulator
    recoemu::Track_t Emulate(const recoemu::Track_t&);

  private:

    const double GetLengthEff(double length) const;
    const double GetAngleEff(double theta, double phi) const;

    RandomDrawTool _totalEff;
    
    double LengthEffParam0;
    double LengthEffParam1;
    double AngleEffParam0;
    double AngleEffParam1;
    double AngleEffParam2;

  };

  /**
     \class larcv::TrackEmuApplyTotalEffFactory
     \brief A concrete factory class for larcv::TrackEmuApplyTotalEff
  */
  class TrackEmuApplyTotalEffFactory : public RecoEmulatorFactoryBase {
  public:
    /// ctor
    TrackEmuApplyTotalEffFactory() { RecoEmulatorFactory::get().add_factory("TrackEmuApplyTotalEff",this); }
    /// dtor
    ~TrackEmuApplyTotalEffFactory() {}
    /// creation method
    RecoEmulatorBase* create() { return new TrackEmuApplyTotalEff; }
  };
  /// Global recoemu::TrackEmuApplyTotalEffFactory to register RecoEmulatorFactory
  static TrackEmuApplyTotalEffFactory __global_TrackEmuApplyTotalEffFactory__;
}

#endif
/** @} */ // end of doxygen group 

