#ifndef LARLITE_MuCST0FINDER_CXX
#define LARLITE_MuCST0FINDER_CXX

#include "MuCST0Finder.h"
#include "DataFormat/track.h"
#include "DataFormat/opflash.h"
#include "DataFormat/cosmictag.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoLineSegment.h"
#include "LArUtil/Geometry.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "FhiclLite/ConfigManager.h"
namespace larlite {

  MuCST0Finder::MuCST0Finder()
  {
    _name = "MuCST0Finder";
  }

  bool MuCST0Finder::initialize() {

    ::fcllite::ConfigManager cfg_mgr;
    cfg_mgr.AddCfgFile(_config_file);
    auto const& main_cfg = cfg_mgr.Config();
    auto const p = main_cfg.get_pset(_name);

    _track_producer          = p.get<std::string>("TrackProducer");
    _opflash_producer_beam   = p.get<std::string>("BeamOpFlashProducer");
    _opflash_producer_cosmic = p.get<std::string>("CosmicOpFlashProducer");
    _flash_trange_start      = p.get<double>("FlashVetoTimeStart");
    _flash_trange_end        = p.get<double>("FlashVetoTimeEnd");
    _num_tracks              = p.get<size_t>("MaxTrackCount");
    _gain_correction         = p.get<std::vector<double> >("GainCorrection");
    _out_track_producer      = p.get<std::string>("OutTrackProducer","");
    
    auto const geo = ::larutil::Geometry::GetME();
    if(geo->NOpDets() != _gain_correction.size()) {
      std::cout << "GainCorrection array size is " << _gain_correction.size() << " != # OpDet " << geo->NOpDets() << std::endl;
      return false;
    }

    _mgr.Configure(main_cfg);

    if(!_out_track_producer.empty())
      std::cout << "Saving tagged track into a producer: " << _out_track_producer << std::endl;

    return true;
  }

  bool MuCST0Finder::analyze(storage_manager* storage) {

    if(!_out_track_producer.empty()) {
      auto ev_track_out = storage->get_data<event_track>(_out_track_producer);
      if(!ev_track_out) {
	std::cerr << "Could not allocate output track data product!" << std::endl;
	return false;
      }
      storage->set_id(storage->run_id(),storage->subrun_id(),storage->event_id());
    }

    _mgr.Reset();
    _result.clear();
    // _mgr.PrintConfig();

    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();

    auto ev_ctag = storage->get_data<event_cosmictag>(Form("MuCSTrackTagger%s",_track_producer.c_str()));
    if(!ev_ctag || ev_ctag->empty()) {
      std::cout << "No cosmictag found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }
    
    auto ev_flash_beam   = storage->get_data<event_opflash> ( _opflash_producer_beam   );
    auto ev_flash_cosmic = storage->get_data<event_opflash> ( _opflash_producer_cosmic );
    if((!ev_flash_cosmic || ev_flash_cosmic->empty()) || (!ev_flash_beam || ev_flash_beam->empty())) {
      std::cout << "No opflash found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }
    
    size_t flash_id=0;
    if(ev_flash_beam) {
      for (size_t n = 0; n < ev_flash_beam->size(); n++) {
	
	auto const& flash = ev_flash_beam->at(n);
	if(flash.Time() < _flash_trange_start || _flash_trange_end < flash.Time())
	  continue;
	
	::flashana::Flash_t f;
	f.x = f.x_err = 0;
	f.y = flash.YCenter();
	f.z = flash.ZCenter();
	f.y_err = flash.YWidth();
	f.z_err = flash.ZWidth();
	f.pe_v.resize(geo->NOpDets());
	f.pe_err_v.resize(geo->NOpDets());
	for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	  unsigned int opdet = geo->OpDetFromOpChannel(i);
	  f.pe_v[opdet] = flash.PE(i) / _gain_correction[i];
	  f.pe_err_v[opdet] = sqrt(flash.PE(i) / _gain_correction[i]);
	}
	f.time = flash.Time();
	f.idx = flash_id;
	++flash_id;
	_mgr.Emplace(std::move(f));
      }
    }

    if(ev_flash_cosmic) {
      for (size_t n = 0; n < ev_flash_cosmic->size(); n++) {
	
	auto const& flash = ev_flash_cosmic->at(n);
	if(flash.Time() < _flash_trange_start || _flash_trange_end < flash.Time())
	  continue;
	
	::flashana::Flash_t f;
	f.x = f.x_err = 0;
	f.y = flash.YCenter();
	f.z = flash.ZCenter();
	f.y_err = flash.YWidth();
	f.z_err = flash.ZWidth();
	f.pe_v.resize(geo->NOpDets());
	f.pe_err_v.resize(geo->NOpDets());
	for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	  unsigned int opdet = geo->OpDetFromOpChannel(i);
	  if(flash.PE(i) == 0.) {
	    f.pe_v[opdet]=-1.;
	    f.pe_err_v[opdet]=-1.;
	  }else{
	    f.pe_v[opdet] = flash.PE(i) / _gain_correction[i] / 0.424;
	    f.pe_err_v[opdet] = sqrt(flash.PE(i) / _gain_correction[i]) / 0.424;
	  }
	}
	f.time = flash.Time();
	f.idx = flash_id;
	++flash_id;
	_mgr.Emplace(std::move(f));
      }
    }
    if(!flash_id) {
      std::cerr << "No relevant flash found... Event: " << storage->event_id() << std::endl;
      return false;
    }
    
    auto ev_track = storage->get_data<event_track>(_track_producer);
    if (!ev_track || ev_track->empty()) {
      std::cerr << "Cosmic tag exists but not track! Cannot happen..." << std::endl;
      throw std::exception();
    }

    // Association
    auto ev_ass = storage->get_data<event_ass>(Form("MuCSTrackTagger%s",_track_producer.c_str()));
    auto const& ctag_ass = ev_ass->association(ev_ctag->id(),ev_track->id());
    if(ctag_ass.empty()) return false;
    if(ctag_ass.size() != 1) {
      std::cerr << "Found >1 association. Skipping an event: " << storage->event_id() << std::endl;
      return false;
    }
    if(ctag_ass[0].size()!=1) {
      std::cerr << "Found >1 associated track. Skipping an event: " << storage->event_id() << std::endl;
      return false;
    }

    auto const& mucs_track  = (*ev_track)[ctag_ass[0][0]];
    ::larlite::geoalgo::Vector mucs_track_start(mucs_track.LocationAtPoint(0));
    ::larlite::geoalgo::Vector mucs_track_end(mucs_track.LocationAtPoint(mucs_track.NumberTrajectoryPoints()-1));
    ::larlite::geoalgo::AABox fidvol(10, (-1.)*(geo->DetHalfHeight())+10., 10.,
			    geo->DetHalfWidth()*2-10., geo->DetHalfHeight()-10., geo->DetLength() -10.);
    if(fidvol.Contain(mucs_track_start) || fidvol.Contain(mucs_track_end)) {
      std::cerr << "Start/End not close enough to the detector edge!" << storage->event_id() << std::endl;
      std::cerr << "Start: (" << mucs_track_start[0] << "," << mucs_track_start[1] << "," << mucs_track_start[2] << ")" << std::endl;
      std::cerr << "End: (" << mucs_track_end[0] << "," << mucs_track_end[1] << "," << mucs_track_end[2] << ")" << std::endl;
      return false;
    }

    ::larlite::geoalgo::Trajectory mucs_geotrj;
    mucs_geotrj.reserve(mucs_track.NumberTrajectoryPoints() - 1);
    for (size_t i = 0; i < mucs_track.NumberTrajectoryPoints(); ++i) {
      ::larlite::geoalgo::Vector pt(mucs_track.LocationAtPoint(i));
      mucs_geotrj.emplace_back(std::move(pt));
    }
    auto mucs_qcluster = ((flashana::LightPath*)(_mgr.GetCustomAlgo("LightPath")))->FlashHypothesis(mucs_geotrj);
    mucs_qcluster.idx = mucs_track.ID();
    _mgr.Emplace(std::move(mucs_qcluster));

    for (size_t trk_idx=0; trk_idx<ev_track->size(); trk_idx++) {

      if(_mgr.QClusterArray().size() >= _num_tracks) break;

      auto const& trk = (*ev_track)[trk_idx];
      mucs_geotrj.resize(trk.NumberTrajectoryPoints(),::larlite::geoalgo::Vector(0.,0.,0.));
      for (size_t pt_idx=0; pt_idx < trk.NumberTrajectoryPoints(); ++pt_idx) {
	auto const& pt = trk.LocationAtPoint(pt_idx);
	mucs_geotrj[pt_idx][0] = pt[0];
	mucs_geotrj[pt_idx][1] = pt[1];
	mucs_geotrj[pt_idx][2] = pt[2];
      }
      auto qcluster = ((flashana::LightPath*)(_mgr.GetCustomAlgo("LightPath")))->FlashHypothesis(mucs_geotrj);
      qcluster.idx = trk.ID();
      _mgr.Emplace(std::move(qcluster));
      
    }

    _result = _mgr.Match();
    
    if(_out_track_producer.empty()) return true;

    auto ev_track_out = storage->get_data<event_track>(_out_track_producer);

    for(auto const& res : _result) {
      int trk_id = _mgr.QClusterArray()[res.tpc_id].idx;
      bool found=false;
      for(auto const& trk : *ev_track) {
	if(trk.ID() != trk_id) continue;
	found=true;
	ev_track_out->push_back(trk);
      }
      if(!found) {
	std::cerr << "Did not find matched track!?" << std::endl;
	throw std::exception();
      }
    }
    return true;
  }

  bool MuCST0Finder::finalize() {

    return true;
  }

}
#endif
