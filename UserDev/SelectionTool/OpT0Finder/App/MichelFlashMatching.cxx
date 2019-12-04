#ifndef LARLITE_MICHELFLASHMATCHING_CXX
#define LARLITE_MICHELFLASHMATCHING_CXX

#include "MichelFlashMatching.h"

#include "DataFormat/track.h"
#include "DataFormat/opflash.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/pfpart.h"

#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoLineSegment.h"
#include "LArUtil/Geometry.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"

namespace larlite {

  MichelFlashMatching::MichelFlashMatching()
    : _flashmatch_tree(nullptr)
  {
    _name = "MichelFlashMatching";
    _fout = 0;

    _e_diff    = 10;
    UseAbsolutePE(false);
    SetStepLength(0.5);

    _opflash_producer = "opflash";
    _track_producer   = "pandoraCosmic";
    _pfpart_producer  = "michel";

  }

  bool MichelFlashMatching::initialize() {

    _flashmatch_tree = new TTree("flashmatch_tree", "");
    _flashmatch_tree->Branch("npe", &_npe, "npe/D");
    _flashmatch_tree->Branch("fy", &_flash_y, "fy/D");
    _flashmatch_tree->Branch("fz", &_flash_z, "fz/D");
    _flashmatch_tree->Branch("tx", &_tpc_x, "tx/D");
    _flashmatch_tree->Branch("ty", &_tpc_y, "ty/D");
    _flashmatch_tree->Branch("tz", &_tpc_z, "tz/D");
    _flashmatch_tree->Branch("ft", &_flash_time, "ft/D");
    _flashmatch_tree->Branch("score", &_score, "score/D");

    return true;
  }

  bool MichelFlashMatching::analyze(storage_manager* storage) {

    _mgr.Reset();
    // _mgr.PrintConfig();

    const ::larutil::Geometry* g = ::larutil::Geometry::GetME();

    auto ev_opflash = storage->get_data<event_opflash>( _opflash_producer  );
    auto ev_pfpart  = storage->get_data<event_pfpart> ( _pfpart_producer   );
    event_track* ev_track = nullptr;
    auto const& track_ass_v = storage->find_one_ass(ev_pfpart->id(), ev_track, "michelkTrackinput");

    if (!ev_opflash || ev_opflash->empty()) {
      std::cout << "No opflash found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }

    if (!ev_track || ev_track->empty()) {
      std::cout << "No 3d michel track found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }

    std::cout << "there are " << ev_track->size() << " tracks in the event!" << std::endl;

    std::cout << "there are " << track_ass_v.size() << " associations [michel pfpart -> pandora track]" << std::endl;
    
    if (track_ass_v.size() == 0)
      return false;

    // For TH2D-- number of flash per event > x PE
    // Number interactions per event > y MeV
    int n_flash = 0;
    int n_int = 0;

    for ( auto & f : *ev_opflash) {
      if (f.TotalPE() > 10 )
        n_flash++;
    }

    // get the 3D track to be used
    auto const& trk = ev_track->at( track_ass_v[0][0] );

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
      tpc_obj.idx = track_ass_v[0][0];
    }
    _mgr.Emplace(std::move(tpc_obj));
    
    // pass optical flashes to the manager
    for (size_t n = 0; n < ev_opflash->size(); n++) {
      
      auto const& flash = ev_opflash->at(n);

      if (flash.TotalPE() < _min_PE)
	continue;
      
      ::flashana::Flash_t f;
      f.x = f.x_err = 0;
      f.y = flash.YCenter();
      f.z = flash.ZCenter();
      f.y_err = flash.YWidth();
      f.z_err = flash.ZWidth();
      f.pe_v.resize(g->NOpDets());
      for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	unsigned int opdet = g->OpDetFromOpChannel(i);
	f.pe_v[opdet] = flash.PE(i);
      }
      f.time = flash.Time();
      f.idx = n;
      
      _mgr.Emplace(std::move(f));
    }
    
    auto const res = _mgr.Match();
    ::larlite::geoalgo::LineSegment line;
    ::larlite::geoalgo::Point_t pt(0, 0, 0);
    ::larlite::geoalgo::GeoAlgo geoalg;
    for (auto const& match : res) {
      auto const& flash = (*ev_opflash)[match.flash_id];
      _flash_y = flash.YCenter();
      _flash_z = flash.ZCenter();
      _tpc_x   = match.tpc_point.x;
      _tpc_y   = match.tpc_point.y;
      _tpc_z   = match.tpc_point.z;
      _npe     = flash.TotalPE();
      _score   = match.score;
      _flash_time = flash.Time();
      _flashmatch_tree->Fill();
    }    
    return true;
    
  }
  
  bool MichelFlashMatching::finalize() {
    if (_fout) {
      _fout->cd();
      _flashmatch_tree->Write();
    }
    return true;
  }
  
}
#endif
