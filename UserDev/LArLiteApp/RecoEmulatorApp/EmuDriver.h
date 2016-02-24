/**
 * \file EmuDriver.h
 *
 * \ingroup RecoEmulatorApp
 * 
 * \brief Class def header for a class EmuDriver
 *
 * @author kazuhiro
 */

/** \addtogroup RecoEmulatorApp

    @{*/

#ifndef __LARLITE_RECOEMULATOR_EMUDRIVER_H__
#define __LARLITE_RECOEMULATOR_EMUDRIVER_H__

#include "Analysis/ana_base.h"
#include "RecoEmulator/TrackEmulatorBase.h"
#include "RecoEmulator/ShowerEmulatorBase.h"
#include "DataFormat/track.h"
#include "DataFormat/shower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include <TRandom.h>
#include "ERToolBackend/ERToolHelperUtil.h"
#include "TTree.h"

namespace larlite {

  /**
     \class EmuDriver
     \brief LArLite driver class (analysis unit) for RecoEmulator
     LArLite driver class (analysis unit) for RecoEmulator package. \n
     Configuration method use FhiclLite configuration file.         \n
     The class is responsible for 3 actions: \n
     0) Convert mctrack and mcshower into recoemu::Track_t and recoemu::Shower_t respectively. \n
     1) Configure & run emulation algorithm. \n
     2) Convert recoemu::Track_t and recoemu::Shower_t into track and shower respectively. \n
     Any change to recoemu::Shower_t and/or recoemu::Track_t should be accompanied with \n
     necessary changes in above conversion methods. It currently does not take care of \n
     track satellite objects such as PID nor calorimetry. Can be added. \n
   */
  class EmuDriver : public ana_base {
  
  public:

    /// Default constructor: name is used to retrieve configuration from a config-file content
    EmuDriver(const std::string name="EmuDriver");

    /// Default destructor
    virtual ~EmuDriver(){}

    /// ana_base::initialize implementation
    bool initialize();

    /// ana_base::analyze implementation
    bool analyze(storage_manager* storage);

    /// ana_base::finalize implementation
    bool finalize();

    /// Configuration method (also configures emulator algorithms)
    void set_config(const std::string cfg_file) { Configure(cfg_file); }

    /// Accessor to mctrack producer (set via config file)
    const std::string& mctrack_producer()  const { return _mctrack_producer; }

    /// Accessor to mcshower producer (set via config file)
    const std::string& mcshower_producer() const { return _mcshower_producer; }

  private:

    /// TTree where to show smearing results
    TTree* _shower_smearing_tree;
    double _dedx_in, _dedx_out;
    double _x_in, _y_in, _z_in, _px_in, _py_in, _pz_in;
    double _x_out, _y_out, _z_out, _px_out, _py_out, _pz_out;
    double _e_in, _e_out;

    /// Configuration method called by EmuDriver::set_config public method
    void Configure(const std::string&);

    /// Boolean flag to keep track of whether or not the instance is configured
    bool _configured;
    std::string _mctrack_producer;  ///< mctrack producer name
    std::string _mcshower_producer; ///< mcshower producer name
    ::recoemu::TrackEmulatorBase*  _track_emu;  ///< Track emulator instance ptr
    ::recoemu::ShowerEmulatorBase* _shower_emu; ///< Shower emulator instance ptr

    /// Method to create recoemu::Track_t from larlite::mctrack
    ::recoemu::Track_t MCTrack2EmuTrack(const mctrack& mct);
    /// Method to create larlite::track from recoemu::Track_t
    track EmuTrack2RecoTrack(const ::recoemu::Track_t& trk);
    /// Method to create recoemu::Shower_t from larlite::mcshower
    ::recoemu::Shower_t MCShower2EmuShower(const mcshower& mct);
    /// Method to create larlite::shower from recoemu::Shower_t
    shower EmuShower2RecoShower(const ::recoemu::Shower_t& trk);

    ERToolHelperUtil *_hutil;

    bool _disable_xshift;
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
