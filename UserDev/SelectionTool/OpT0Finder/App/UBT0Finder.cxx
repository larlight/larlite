#ifndef LARLITE_UBT0FINDER_CXX
#define LARLITE_UBT0FINDER_CXX

#include "UBT0Finder.h"
#include "DataFormat/track.h"
#include "DataFormat/opflash.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/mctrack.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoLineSegment.h"
namespace larlite {

  UBT0Finder::UBT0Finder()
    : _track_tree(nullptr)
    , _tree(nullptr)
  {
    _name="UBT0Finder";
    _fout=0;
  }

  bool UBT0Finder::initialize() {

    _track_tree = new TTree("track_tree","");
    _track_tree->Branch("trk_time",&_trk_time,"trk_time/D");
    _track_tree->Branch("trk_x",&_trk_x,"trk_x/D");

    _tree = new TTree("flash_tree","");
    _tree->Branch("npe",&_npe,"npe/D");
    _tree->Branch("fy",&_flash_y,"fy/D");
    _tree->Branch("fz",&_flash_z,"fz/D");
    _tree->Branch("tx",&_tpc_x,"tx/D");
    _tree->Branch("ty",&_tpc_y,"ty/D");
    _tree->Branch("tz",&_tpc_z,"tz/D");
    _tree->Branch("ft",&_flash_time,"ft/D");
    _tree->Branch("mct",&_mc_time,"mct/D");
    _tree->Branch("mcx",&_mc_x,"mcx/D");
    _tree->Branch("mcy",&_mc_y,"mcy/D");
    _tree->Branch("mcz",&_mc_z,"mcz/D");
    _tree->Branch("mc_dx",&_mc_dx,"mc_dx/D");
    _tree->Branch("score",&_score,"score/D");

    return true;
  }
  
  bool UBT0Finder::analyze(storage_manager* storage) {

    _mgr.Reset();

    auto ev_flash = storage->get_data<event_opflash>("opflash");

    if(!ev_flash || ev_flash->empty()) {
      std::cout<<"No opflash found. Skipping event: "<<storage->event_id()<<std::endl;
      return false;
    }

    //auto ev_track = storage->get_data<event_track>("pandoraCosmicKHit");
    auto ev_track = storage->get_data<event_track>("trackkalmanhit");
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
    
    if(!_use_mc) {
      if (!ev_track || ev_track->empty()) return false;
      for (size_t n=0; n < ev_track->size(); n++){
	auto const& trk = ev_track->at(n);
	    
	::flashana::QCluster_t tpc_obj;
	tpc_obj.reserve(trk.NumberTrajectoryPoints()-1);
	for(size_t i=0; i < (trk.NumberTrajectoryPoints()-1); ++i) {
	  
	  auto const& pt1 = trk.LocationAtPoint(i);
	  auto const& pt2 = trk.LocationAtPoint(i+1);
	  
	  ::flashana::QPoint_t pt;
	  
	  double dx = pt2[0] - pt1[0];
	  double dy = pt2[1] - pt1[1];
	  double dz = pt2[2] - pt1[2];
	  
	  double dist = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
	  pt.q = (dist * 10. * 2.);
	  pt.x = pt1[0] + dx/2.;
	  pt.y = pt1[1] + dy/2.;
	  pt.z = pt1[2] + dz/2.;
	  
	  tpc_obj.emplace_back(pt);
	  tpc_obj.idx = n;
	}
	_mgr.Emplace(std::move(tpc_obj));
      }
      
    }
    // use MC tracks
    else{
      if (!ev_mctrack || ev_mctrack->empty()) return false;
      for (size_t n=0; n < ev_mctrack->size(); n++){
	auto const& trk = ev_mctrack->at(n);

	::flashana::QCluster_t tpc_obj;

	if(trk.size()>=2) {
	  
	  // per track calculate the shift in x-direction
	  // so that the x-position is what would be seen
	  // in the TPC, not the truth x-position
	  double event_time = trk[trk.size()-1].T();
	  double det_frame_period = 1.6E6; // ns
	  double det_width = 256.; // cm
	  double shift_x = event_time * (det_width/ det_frame_period);
	  tpc_obj.reserve(trk.size()-1);

	  _trk_time = trk[0].T()/1000.;
	  _trk_x    = trk[0].X() + shift_x;
	  _track_tree->Fill();
	  
	  for(size_t i=0; i < (trk.size()-1); ++i) {
	    
	    auto const& pt1 = trk[i].Position();
	    auto const& pt2 = trk[i+1].Position();
	    
	    ::flashana::QPoint_t pt;
	    
	    double dx = pt2[0] - pt1[0];
	    double dy = pt2[1] - pt1[1];
	    double dz = pt2[2] - pt1[2];
	    
	    pt.q = (trk[i].E() - trk[i+1].E());
	    pt.x = pt1[0] + dx/2. + shift_x;
	    pt.y = pt1[1] + dy/2.;
	    pt.z = pt1[2] + dz/2.;
	    
	  tpc_obj.push_back(pt);
	  tpc_obj.idx = n;
	  }
	_mgr.Emplace(std::move(tpc_obj));
	}// if the track is at least 2 elements long
      }// for all tracks
    }

    for (size_t n=0; n < ev_flash->size(); n++){

      auto const& flash = ev_flash->at(n);

      ::flashana::Flash_t f;
      f.x = f.x_err = 0;
      f.y = flash.YCenter();
      f.z = flash.ZCenter();
      f.y_err = flash.YWidth();
      f.z_err = flash.ZWidth();
      f.pe_v.reserve(32);
      for(unsigned int i=0; i<32; i++)
	f.pe_v.push_back(flash.PE(i));
      f.time = flash.Time();
      f.idx = n;

      _mgr.Emplace(std::move(f));
    }

    /*
    for(auto const& calo : *ev_calo) {

      ::flashana::TPCObject_t tpc_obj;

      auto const& dedx = calo.dEdx();
      auto const& dx   = calo.TrkPitchVec();

      std::cout<<dedx.size()<<" : "<<dx.size()<<std::endl;

    }
    */

    auto const res = _mgr.Match();
    ::geoalgo::LineSegment line;
    ::geoalgo::Point_t pt(0,0,0);
    ::geoalgo::GeoAlgo geoalg;
    for(auto const& match : res) {
      auto const& flash = (*ev_flash)[match.flash_id];
      _flash_y = flash.YCenter();
      _flash_z = flash.ZCenter();
      _tpc_x   = match.tpc_point.x;
      _tpc_y   = match.tpc_point.y;
      _tpc_z   = match.tpc_point.z;
      _npe     = flash.TotalPE();
      _score   = match.score;
      _flash_time = flash.Time();
      _mc_time = _mc_x = _mc_y = _mc_z = -1;
      if(_use_mc) {
	auto const& mct = (*ev_mctrack)[match.tpc_id];
	_mc_time = mct[0].T() * 1.e-3;
	double min_dist = 1e12;
	pt[0] = _tpc_x;
	pt[1] = _tpc_y;
	pt[2] = _tpc_z;
	double min_x=1e9;
	double max_x=0;
	for(size_t i=0; i<mct.size()-1; ++i) {
	  auto const& step1 = mct[i];
	  auto const& step2 = mct[i+1];
	  line.Start(step1.X(),step1.Y(),step1.Z());
	  line.End(step2.X(),step2.Y(),step2.Z());
	  auto const closest_pt = geoalg.ClosestPt(line,pt);
	  double dist = closest_pt.SqDist(pt);
	  if(dist < min_dist) {
	    min_dist = dist;
	    _mc_x = closest_pt[0];
	    _mc_y = closest_pt[1];
	    _mc_z = closest_pt[2];
	  }

	  if(step1.X() < min_x) min_x = step1.X();
	  if(step1.X() > max_x) max_x = step1.X();
	}
	_mc_dx = max_x - min_x;
      }
      _tree->Fill();
    }
    return true;
  }

  bool UBT0Finder::finalize() {
    if(_fout) {
      _fout->cd();
      _tree->Write();
      _track_tree->Write();
    }
    return true;
  }

}
#endif
