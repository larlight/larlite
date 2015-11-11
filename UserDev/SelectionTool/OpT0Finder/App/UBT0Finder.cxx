#ifndef LARLITE_UBT0FINDER_CXX
#define LARLITE_UBT0FINDER_CXX

#include "UBT0Finder.h"
#include "DataFormat/track.h"
#include "DataFormat/opflash.h"
#include "DataFormat/ophit.h"
#include "DataFormat/calorimetry.h"
#include "DataFormat/mctrack.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoLineSegment.h"
#include "LArUtil/Geometry.h"
namespace larlite {

  UBT0Finder::UBT0Finder()
    : _track_tree(nullptr)
    , _tree(nullptr)
    , _int_tree(nullptr)
    , _eff_tree(nullptr)
    , _time_diff(nullptr)
  {
    _name="UBT0Finder";
    _fout=0;
    _npe_test = 0; 

    _n_int     = 0;
    _n_int_tot = 0;
    _n_matches = 0;
    _e_diff    = 10;
  }

  bool UBT0Finder::initialize() {

    _time_diff = new TH1D("time_diff","Matched Flash vs. MCTrack",100,0,500);

    _flash_v_x = new TH2D("_flash_v_x","OpFlash Z Width vs TPC x point",100,0,256,100,0,1450);
    _nflash_v_nint = new TH2D("_nflash_v_nint","OpFlash with PE > 10 vs Nint/event ",50,0,50,30,0,30);

    _int_tree = new TTree("int_tree","");
    _int_tree->Branch("_t0",&_t0,"t0/D");
    _int_tree->Branch("_n_pe",&_n_pe,"n_pe/D");

    _track_tree = new TTree("track_tree","");
    _track_tree->Branch("trk_time",&_trk_time,"trk_time/D");
    _track_tree->Branch("trk_max_x",&_trk_max_x,"trk_max_x/D");
    _track_tree->Branch("trk_min_x",&_trk_min_x,"trk_min_x/D");
    _track_tree->Branch("trk_shift",&_trk_shift,"trk_shift/D");
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
    _tree->Branch("mcE",&_mc_edep,"mcE/D");
    _tree->Branch("mcx",&_mc_x,"mcx/D");
    _tree->Branch("mcy",&_mc_y,"mcy/D");
    _tree->Branch("mcz",&_mc_z,"mcz/D");
    _tree->Branch("mc_dx",&_mc_dx,"mc_dx/D");
    _tree->Branch("score",&_score,"score/D");
    _tree->Branch("trk_shift",&_trk_shift,"trk_shift/D");

    if (_eff_tree) delete _eff_tree;
    _eff_tree = new TTree("_eff_tree","Efficiency Tree");
    _eff_tree->Branch("_matched",&_matched,"matched/I");
    _eff_tree->Branch("_mc_time",&_mc_time,"mc_time/D");
    _eff_tree->Branch("_mc_edep",&_mc_edep,"mc_edep/D");
    _eff_tree->Branch("_flash_time",&_flash_time,"flash_time/D");
    _eff_tree->Branch("_npe",&_npe,"flash_pe/D");
    _eff_tree->Branch("_npts",&_npts,"_npts/D");
    _eff_tree->Branch("trk_max_x",&_trk_max_x,"trk_max_x/D");
    _eff_tree->Branch("trk_min_x",&_trk_min_x,"trk_min_x/D");
    _eff_tree->Branch("trk_max_abs_x",&_trk_max_abs_x,"trk_max_abs_x/D");
    _eff_tree->Branch("trk_min_abs_x",&_trk_min_abs_x,"trk_min_abs_x/D");
    _eff_tree->Branch("trk_shift",&_trk_shift,"trk_shift/D");

    return true;
  }
  
  bool UBT0Finder::analyze(storage_manager* storage) {

    _mgr.Reset();
    const ::larutil::Geometry* g = ::larutil::Geometry::GetME();

    std::cout<<"\nNew event!"<<std::endl ;

    auto ev_flash = storage->get_data<event_opflash>("opflash");// opflash");
    auto ev_hit = storage->get_data<event_ophit>("satOpFlash");// opflash");

    if(!ev_flash || ev_flash->empty()) {
      std::cout<<"No opflash found. Skipping event: "<<storage->event_id()<<std::endl;
      return false;
    }

    int n_flash = 0;
    int n_int = 0;



    for ( auto & f : *ev_flash) {
	if (f.TotalPE() > 10 )
	    n_flash++;
	}

    //auto ev_track = storage->get_data<event_track>("pandoraCosmicKHit");
    auto ev_track = storage->get_data<event_track>("trackkalmanhit");
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
    
    if(!_use_mc) {
      if (!ev_track || ev_track->empty()) return false;
      for (size_t n=0; n < ev_track->size(); n++){
	std::vector<int> ids ;

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
      //ahack 110915
      std::vector<int> usedIDs;
      usedIDs.resize(0);

      if (!ev_mctrack || ev_mctrack->empty()) return false;

      for (size_t n=0; n < ev_mctrack->size(); n++){

//	std::cout<<"Ancestors: "<<ev_mctrack->at(n).AncestorTrackID()<<std::endl;
	//ahack 110915, don't repeat study for used IDs
	bool used = false ;
	for(size_t u=0; u < usedIDs.size(); u++){
	    if( ev_mctrack->at(n).AncestorTrackID() == usedIDs[u] )
		used = true;
	    }

	//ahack 110915
	auto const& trk = ev_mctrack->at(n);

	::flashana::QCluster_t tpc_obj;

	if(trk.size()>=2) {
	  
	  // per track calculate the shift in x-direction
	  // so that the x-position is what would be seen
	  // in the TPC, not the truth x-position
	  double event_time = trk[0].T(); // ns
	  double det_drift_time = 2.2E6; // ns
	  double det_width = 256.; // cm
	  double shift_x = event_time * (det_width/det_drift_time);
	  tpc_obj.reserve(trk.size()-1);

	  _trk_time = trk[0].T()/1000.;
	  _trk_x    = trk[0].X();
	  _trk_shift = shift_x;
	  _trk_min_x = 1036.;
	  _trk_max_x = -1036.;
	  for(size_t i=0; i < trk.size(); ++i) {
	    if (trk[i].X() > _trk_max_x) { _trk_max_x = trk[i].X(); }
	    if (trk[i].X() < _trk_min_x) { _trk_min_x = trk[i].X(); }
	  }

	  _track_tree->Fill();
	 //ahack 11/09/15.  Find all tracks with same ancestor ID and add them to the tpc_obj
	 if(!used){

	   _t0 = trk.AncestorStart().T() ;
	   _n_pe = 0 ;

	   for(auto const & f : *ev_hit ){

	      if(  f.PeakTime() >_t0/1000 -1 && f.PeakTime() < _t0/1000 +1)
		_n_pe += f.PE();
	   
	    }

	   std::vector<int> ids ;
	   ids.resize(0);
	   auto const & ancID = trk.AncestorTrackID() ;
	   for (size_t m=0; m < ev_mctrack->size(); m++){

	      auto const & trk2 = ev_mctrack->at(m);

	      if( ancID == trk2.AncestorTrackID() && trk2.size() > 2){
		usedIDs.push_back(ancID);
	  	ids.push_back(m) ;
		}
	      }
	    

	  // Now loop over all mctracks that share an ancestor and treat
	  // them as one interaction
	  double e_diff = 0;
	  for(size_t j=0; j < ids.size(); j++) {
	      auto const & trk3 = ev_mctrack->at(ids[j]);
	      ::flashana::QPoint_t pt;

//	    if( trk3.size() != 0 ){
		for(size_t i=0; i < (trk3.size()-1); ++i) {

	    	  auto const& pt1 = trk3[i].Position();
	    	  auto const& pt2 = trk3[i+1].Position();
	    	  
	    	  double dx = pt2[0] - pt1[0];
	    	  double dy = pt2[1] - pt1[1];
	    	  double dz = pt2[2] - pt1[2];
	    	  
	    	  pt.q = (trk3[i].E() - trk3[i+1].E());
	    	  pt.x = pt1[0] + dx/2. + shift_x;
	    	  pt.y = pt1[1] + dy/2.;
	    	  pt.z = pt1[2] + dz/2.;
	    	  
	    	  tpc_obj.push_back(pt);
	    	  tpc_obj.idx = n;
	    	  }
	        e_diff += (trk3[0].E() - trk3[trk3.size()-1].E()); 
	      }

//	    }
	    
	    _mgr.Emplace(std::move(tpc_obj));
	    _n_int_tot++;
	    
//	    std::cout<<"What's ediff ? "<<e_diff<<std::endl ;
	    if( e_diff > _e_diff ) { 
		_n_int++ ;
		n_int++;
		}
		
	  _int_tree->Fill();		
          }// if index has not already been used
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
      f.pe_v.resize(g->NOpDets());
      for(unsigned int i=0; i<f.pe_v.size(); i++) {
	unsigned int opdet = g->OpDetFromOpChannel(i);
	f.pe_v[opdet] = flash.PE(i);
      }
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
      _n_matches++;
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
	
      _flash_v_x->Fill(match.tpc_point.x, flash.ZWidth());
	
      if(_use_mc) {
	
	auto const& mct = (*ev_mctrack)[match.tpc_id];
	_mc_time = mct[0].T() * 1.e-3;
	double event_time = mct[0].T(); // ns
	double det_drift_time = 2.2E6; // ns
	double det_width = 256.; // cm
	_trk_shift = event_time * (det_width/det_drift_time);
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
      if( mct[0].E() - mct[mct.size()-1].E() > _e_diff )
	_time_diff->Fill(1000*(_flash_time - _mc_time)); //
      }
      _tree->Fill();

    }// for all matches

    
    // make an entry of the flash-matching results for every MCTrack
    if(!_use_mc)
      return true;

    _npts = 0 ; 
    for (size_t n=0; n < ev_mctrack->size(); n++){
      auto const& mct = ev_mctrack->at(n);
      // ignore tracks with < 2 steps
      if (mct.size() < 2) continue;
      // find the flash that was matched for this MCTrack (if any)

      _mc_time = mct[0].T() * 1.e-3;
      _mc_edep = mct[0].E()-mct[mct.size()-1].E();
      double event_time = mct[0].T(); // ns
      double det_drift_time = 2.2E6; // ns
      double det_width = 256.; // cm
      _trk_shift = event_time * (det_width/det_drift_time);
      _trk_min_x =  1036.;
      _trk_max_x = -1036.;
      _trk_min_abs_x =  2036.;
      _trk_max_abs_x = -2036.;

      for(size_t i=0; i < (mct.size()-1); ++i) {
	
	auto const& pt1 = mct[i].Position();
	auto const& pt2 = mct[i+1].Position();
	
	if ( (pt1[0]+_trk_shift) > _trk_max_abs_x) _trk_max_abs_x = pt1[0]+_trk_shift;
	if ( (pt1[0]+_trk_shift) < _trk_min_abs_x) _trk_min_abs_x = pt1[0]+_trk_shift;
	    
	double dx = pt2[0] - pt1[0];
	double x  = pt1[0] + dx/2. + _trk_shift;
	
	if (x > _trk_max_x) _trk_max_x = x;
	if (x < _trk_min_x) _trk_min_x = x;			   
      }

      _matched    = 0 ;
      _npe        = 0;
      _npts = mct.size() ;

      for(auto const& match : res) {
	if (match.tpc_id == n){
	  _matched = 1;
	  auto const& flash = (*ev_flash)[match.flash_id];
	  _flash_time  = flash.Time();
	  _npe = flash.TotalPE();
	}
      }

      _eff_tree->Fill();
    }// for all MCTracks

    _nflash_v_nint->Fill(n_int, n_flash);

    if(_npts == 0 )
      _eff_tree->Fill();

    return true;
  }

  bool UBT0Finder::finalize() {
    if(_fout) {

      std::cout<<"Number of int with > "<<_e_diff<<" MeV and total : "<<_n_int<<", "<<_n_int_tot<<std::endl ;

      _fout->cd();

      _flash_v_x->GetXaxis()->SetTitle("TPC x position of match");
      _flash_v_x->GetYaxis()->SetTitle("Flash Z Width");
      _time_diff->GetXaxis()->SetTitle("Delta T [ns]" );

      std::cout<<"\n% Total interactions that were matched      : "<<float(_n_matches)/_n_int *100 <<"%, "<<_n_matches<<", "<<_n_int<<std::endl;
      std::cout<<"% Matches whose deltaT falls within 40-120ns: "<<_time_diff->Integral(8,24)/_n_int *100<<"%, "<<_time_diff->Integral(8,24)<<", "<<_n_int<<std::endl ;
      std::cout<<"% Correctness (good matches/total matches)  : " <<_time_diff->Integral(8,24)/_n_matches*100<<"%, "<<_time_diff->Integral(8,24)<<", "<<_n_matches<<std::endl;



      _tree->Write();
      _int_tree->Write();
      _track_tree->Write();
      _eff_tree->Write();
      _flash_v_x->Write();
      _nflash_v_nint->Write();
      _time_diff->Write();
    }
    return true;
  }

}
#endif
