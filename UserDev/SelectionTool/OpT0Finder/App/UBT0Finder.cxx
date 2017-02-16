#ifndef LARLITE_UBT0FINDER_CXX
#define LARLITE_UBT0FINDER_CXX

#include "UBT0Finder.h"
#include "DataFormat/track.h"
#include "DataFormat/opflash.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/trigger.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoLineSegment.h"
#include "LArUtil/Geometry.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
namespace larlite {

  UBT0Finder::UBT0Finder()
  {
    _name = "UBT0Finder";
    _shift_flash_time = 0.;
  }

  bool UBT0Finder::initialize() {

    ::fcllite::ConfigManager cfg_mgr;
    cfg_mgr.AddCfgFile(_config_file);
    auto const& main_cfg = cfg_mgr.Config();
    auto const p = main_cfg.get_pset(_name);

    _use_mc = p.get<bool>("UseMC");
    _opflash_producer_beam = p.get<std::string>("BeamOpFlashProducer");
    _opflash_producer_cosmic = p.get<std::string>("CosmicOpFlashProducer");
    _trigger_producer = p.get<std::string>("TriggerProducer");
    _track_producer = p.get<std::string>("TrackProducer");

    _mgr.Configure(main_cfg);

    return true;
  }

  bool UBT0Finder::analyze(storage_manager* storage) {

    _mgr.Reset();
    _result.clear();
    // _mgr.PrintConfig();

    const ::larutil::Geometry* g = ::larutil::Geometry::GetME();

    auto ev_flash_beam   = storage->get_data<event_opflash> ( _opflash_producer_beam   ); // opflash");
    auto ev_flash_cosmic = storage->get_data<event_opflash> ( _opflash_producer_cosmic ); // opflash");

    if((!ev_flash_beam || ev_flash_beam->empty()) && (!ev_flash_cosmic || ev_flash_cosmic->empty())) {
      std::cout << "No opflash found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }

    if (!_use_mc) {

      //auto ev_track = storage->get_data<event_track>("pandoraCosmicKHit");
      auto ev_track = storage->get_data<event_track>(_track_producer);

      if (!ev_track || ev_track->empty()) return false;

      for (size_t n = 0; n < ev_track->size(); n++) {
        std::vector<int> ids ;

        auto const& trk = ev_track->at(n);

        ::flashana::QCluster_t tpc_obj;
        tpc_obj.reserve(trk.NumberTrajectoryPoints() - 1);

        for (size_t i = 0; i < (trk.NumberTrajectoryPoints() - 1); ++i) {

          auto const& pt1 = trk.LocationAtPoint(i);
          auto const& pt2 = trk.LocationAtPoint(i + 1);

          ::flashana::QPoint_t pt;

          double dx = pt2[0] - pt1[0];
          double dy = pt2[1] - pt1[1];
          double dz = pt2[2] - pt1[2];

          double dist = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
          pt.q = (dist * 10. * 2.);
          pt.x = pt1[0] + dx / 2.;
          pt.y = pt1[1] + dy / 2.;
          pt.z = pt1[2] + dz / 2.;

          tpc_obj.emplace_back(pt);
          tpc_obj.idx = n;
        }
        _mgr.Emplace(std::move(tpc_obj));
      }
    }
    // use MC tracks
    else {

      auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
      auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
      if (!ev_mctrack || ev_mctrack->empty()) return false;

      // Get MC QCluster list-- declared here for adding in light path
      // Fills _q_cluster_v inside MCQCluster
      std::vector<flashana::QCluster_t> qcluster_v;
      std::vector<flashana::MCSource_t> source_v;

      auto mcqclustering = (flashana::MCQCluster*)(_mgr.GetCustomAlgo("MCQCluster"));
      auto const& lightpath = *((flashana::LightPath*)(_mgr.GetCustomAlgo("LightPath")));

      auto ev_trigger = storage->get_data<trigger>(_trigger_producer);
      if(ev_trigger) {
	mcqclustering->SetTriggerTime(ev_trigger->TriggerTime());
      }
      
      mcqclustering->Construct(*ev_mctrack, *ev_mcshower, lightpath);
      mcqclustering->Swap(std::move(qcluster_v), std::move(source_v));

      for (auto& qcluster : qcluster_v) 
        if ( qcluster.idx != -1 )
          _mgr.Emplace(std::move(qcluster));

    }//else

    size_t flash_id=0;
    if(ev_flash_beam) {
      for (size_t n = 0; n < ev_flash_beam->size(); n++) {
	
	auto const& flash = ev_flash_beam->at(n);
	
	::flashana::Flash_t f;
	f.x = f.x_err = 0;
	f.y = flash.YCenter();
	f.z = flash.ZCenter();
	f.y_err = flash.YWidth();
	f.z_err = flash.ZWidth();
	f.pe_v.resize(g->NOpDets());
	f.pe_err_v.resize(g->NOpDets());
	for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	  unsigned int opdet = g->OpDetFromOpChannel(i);
	  f.pe_v[opdet] = flash.PE(i);
	  f.pe_err_v[opdet] = sqrt(flash.PE(i));
	}
	f.time = flash.Time() + _shift_flash_time;
	f.idx = flash_id;
	++flash_id;
	_mgr.Emplace(std::move(f));
      }
    }

    if(ev_flash_cosmic) {
      for (size_t n = 0; n < ev_flash_cosmic->size(); n++) {
	
	auto const& flash = ev_flash_cosmic->at(n);
	
	::flashana::Flash_t f;
	f.x = f.x_err = 0;
	f.y = flash.YCenter();
	f.z = flash.ZCenter();
	f.y_err = flash.YWidth();
	f.z_err = flash.ZWidth();
	f.pe_v.resize(g->NOpDets());
	f.pe_err_v.resize(g->NOpDets());
	for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	  unsigned int opdet = g->OpDetFromOpChannel(i);
	  if(flash.PE(i) == 0.) {
	    f.pe_v[opdet]=-1.;
	    f.pe_err_v[opdet]=-1.;
	  }else{
	    f.pe_v[opdet] = flash.PE(i) / 0.424;
	    f.pe_err_v[opdet] = sqrt(flash.PE(i)) / 0.424;
	  }
	}
	f.time = flash.Time() + _shift_flash_time;
	f.idx = flash_id;
	++flash_id;
	_mgr.Emplace(std::move(f));
      }
    }

    _result = _mgr.Match();
    // std::cout << "UBT0FINDER: " << _result.size() << " matches found." << std::endl;
    return true;
  }

  bool UBT0Finder::finalize() {

    return true;
  }

}
#endif
