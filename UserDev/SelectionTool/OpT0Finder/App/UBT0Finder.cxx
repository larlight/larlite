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
    :  _photon_bomb(false),_x_pb(0.0),_y_pb(0.0),_z_pb(0.0),_nphoton_pb(0) 
    , _int_tree(nullptr)
    , _track_tree(nullptr)
    , _flashmatch_tree(nullptr)
    , _photlib_tree_config("")
    , _photlib_tree(nullptr)
    , _eff_tree(nullptr)
    , _flash_tree(nullptr)
    , _nflash_v_nint(nullptr)
    , _time_diff(nullptr)
  {
    _name = "UBT0Finder";
    _fout = 0;

    _e_diff    = 10;
    UseAbsolutePE(false);
    SetStepLength(0.5) ;
  }

  void UBT0Finder::PhotonBomb (const double& x, const double& y, const double& z, const int& n)
  {
    std::cout << "UBT0Finder:PhotonBomb(): Photon Bomb mode." << std::endl;
    _photon_bomb = true;
    _x_pb = x; _y_pb = y; _z_pb = z;
    _nphoton_pb = n;
  }

  bool UBT0Finder::initialize() {

    _time_diff = new TH1D("time_diff", "Matched Flash vs. MCTrack", 100, 0, 500);

    _nflash_v_nint = new TH2D("_nflash_v_nint", "OpFlash with PE > 10 vs Nint/event ", 50, 0, 50, 30, 0, 30);

    _int_tree = new TTree("int_tree", "");
    _int_tree->Branch("_t0", &_t0, "t0/D");
    _int_tree->Branch("_n_pe", &_n_pe, "n_pe/D");
    _int_tree->Branch("_int_e", &_int_e, "int_e/D");
    _int_tree->Branch("_n_flash", &_n_flash, "n_flash/D");

    _track_tree = new TTree("track_tree", "");
    _track_tree->Branch("trk_time", &_trk_time, "trk_time/D");
    _track_tree->Branch("trk_max_x", &_trk_max_x, "trk_max_x/D");
    _track_tree->Branch("trk_min_x", &_trk_min_x, "trk_min_x/D");
    _track_tree->Branch("trk_shift", &_trk_shift, "trk_shift/D");
    _track_tree->Branch("trk_x", &_trk_x, "trk_x/D");

    _flashmatch_tree = new TTree("flashmatch_tree", "");
    _flashmatch_tree->Branch("npe", &_npe, "npe/D");
    _flashmatch_tree->Branch("tpcID", &_tpcID, "tpcID/I");
    _flashmatch_tree->Branch("event", &_event, "event/I");
    _flashmatch_tree->Branch("fy", &_flash_y, "fy/D");
    _flashmatch_tree->Branch("fz", &_flash_z, "fz/D");
    _flashmatch_tree->Branch("tx", &_tpc_x, "tx/D");
    _flashmatch_tree->Branch("ty", &_tpc_y, "ty/D");
    _flashmatch_tree->Branch("tz", &_tpc_z, "tz/D");
    _flashmatch_tree->Branch("tt", &_tpc_t, "tt/D");
    _flashmatch_tree->Branch("ft", &_flash_time, "ft/D");
    _flashmatch_tree->Branch("mct", &_mc_time, "mct/D");
    _flashmatch_tree->Branch("mcE", &_mc_edep, "mcE/D");
    _flashmatch_tree->Branch("mcx", &_mc_x, "mcx/D");
    _flashmatch_tree->Branch("mcy", &_mc_y, "mcy/D");
    _flashmatch_tree->Branch("mcz", &_mc_z, "mcz/D");
    _flashmatch_tree->Branch("mc_dx", &_mc_dx, "mc_dx/D");
    _flashmatch_tree->Branch("length", &_mc_length, "length/D");
    _flashmatch_tree->Branch("trajx", &_mc_trajx);
    _flashmatch_tree->Branch("trajy", &_mc_trajy);
    _flashmatch_tree->Branch("trajz", &_mc_trajz);
    _flashmatch_tree->Branch("score", &_score, "score/D");
    _flashmatch_tree->Branch("trk_shift", &_trk_shift, "trk_shift/D");

    if (_eff_tree) delete _eff_tree;
    _eff_tree = new TTree("_eff_tree", "Efficiency Tree");
    _eff_tree->Branch("_matched", &_matched, "matched/I");
    _eff_tree->Branch("_mc_time", &_mc_time, "mc_time/D");
    _eff_tree->Branch("_mc_edep", &_mc_edep, "mc_edep/D");
    _eff_tree->Branch("_flash_time", &_flash_time, "flash_time/D");
    _eff_tree->Branch("_npe", &_npe, "flash_pe/D");
    _eff_tree->Branch("_npts", &_npts, "_npts/D");
    _eff_tree->Branch("trk_max_x", &_trk_max_x, "trk_max_x/D");
    _eff_tree->Branch("trk_min_x", &_trk_min_x, "trk_min_x/D");
    _eff_tree->Branch("trk_max_abs_x", &_trk_max_abs_x, "trk_max_abs_x/D");
    _eff_tree->Branch("trk_min_abs_x", &_trk_min_abs_x, "trk_min_abs_x/D");
    _eff_tree->Branch("trk_shift", &_trk_shift, "trk_shift/D");

    if (_flash_tree) delete _flash_tree;
    _flash_tree = new TTree("_flash_tree", "Efficiency Tree");
//    _flash_tree->Branch("_nflash", &_nflash, "nflash/I");
    _flash_tree->Branch("_npe", &_npe, "npe/D");

    std::cout << "UBT0Finder.cxx: In inititalize(): "  << std::endl;
    Configure((const ::fcllite::PSet )"UBT0Finder"); 
    if (_photlib_tree_config != NULL) 
      {
	if (_photlib_tree) delete _photlib_tree;
	_photlib_tree = new TTree("_photlib_tree", "PhotonLibrary Tree");
	_photlib_tree->Branch("pvl_x", &_pvl_x, "pvl_x/D");
	_photlib_tree->Branch("pvl_y", &_pvl_y, "pvl_y/D");
	_photlib_tree->Branch("pvl_z", &_pvl_z, "pvl_z/D");
	_photlib_tree->Branch("pvl_pmt", &_pvl_pmt, "pvl_pmt/I");
	_photlib_tree->Branch("pvl_vis", &_pvl_vis, "pvl_vis/D");
	if (_photlib_tree_config == "lib") 
	  Fill_PVL_Tree(); // Fill the (badly-binned) PVL, as from input file.
      }
    return true;
  }

  void UBT0Finder::Configure(const ::fcllite::PSet &pset) {
    // This doesn't work, cuz .... not finding actual fcl file, as set in _config_file of Manager. So, default to false.
    _photlib_tree_config = pset.get<std::string>("PhotonLibTTree",std::string("seen"));  // "lib" or "seen" or ""
    std::cout << "UBT0Finder.cxx: PhotonVisLibrary Tree ?: " << _photlib_tree_config << std::endl;
  }

  void UBT0Finder::Fill_PVL_Tree() {


    auto geom = ::larutil::Geometry::GetME();
    double dw = 2 * geom->DetHalfWidth();
    double dh = 2 * geom->DetHalfHeight();
    double dl = 1 * geom->DetLength();


    for ( size_t ipmt = 0; ipmt < 32; ++ipmt) {
      for ( size_t ix = 0; ix <= 100; ++ix) {
	for ( size_t iy = 0; iy <= 100; ++iy) {
	  for ( size_t iz = 0; iz <= 100; ++iz) {
	    _pvl_x = dw/100.0*ix; _pvl_y = dh/100.*iy - dh/2.; _pvl_z = dl/100.0*iz; _pvl_pmt = ipmt; 
	    _pvl_vis = ::phot::PhotonVisibilityService::GetME().GetVisibility( _pvl_x, _pvl_y, _pvl_z, _pvl_pmt) ;
	    _photlib_tree->Fill();
	  }
	}
      }
    }


  }

  void UBT0Finder::Fill_PVL_Tree(const flashana::QCluster_t& qcl) {

    for (auto const & qpt : qcl)
    {
      _pvl_x = qpt.x;
      _pvl_y = qpt.y;
      _pvl_z = qpt.z;
      for (int ipmt=0; ipmt<32; ipmt++)
	{
	  _pvl_pmt = ipmt;
	  _pvl_vis = ::phot::PhotonVisibilityService::GetME().GetVisibility( _pvl_x, _pvl_y, _pvl_z, _pvl_pmt) ;
	  _photlib_tree->Fill();
	}
    }

  }

  bool UBT0Finder::analyze(storage_manager* storage) {

    _mgr.Reset();
    const ::larutil::Geometry* g = ::larutil::Geometry::GetME();

    auto ev_flash = storage->get_data<event_opflash>("opflash");// opFlash"); // EC: change capitalization in these 2 lines.
    auto ev_hit = storage->get_data<event_ophit>    ("opflash"); // opFlash");



    if (!ev_flash || ev_flash->empty()) {
      std::cout << "No opflash found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }

    const double det_drift_time = 2.3E6; // ns
    const double det_width = 256.35; // cm


    // For TH2D-- number of flash per event > x PE
    // Number interactions per event > y MeV
    int n_flash = 0;
    int n_int = 0;

    for ( auto & f : *ev_flash) {
      if (f.TotalPE() > 10 )
        n_flash++;
    }


    for (auto const & f : *ev_flash){
      _npe = f.TotalPE();
      _flash_tree->Fill();
      }

    //auto ev_track = storage->get_data<event_track>("pandoraCosmicKHit");
    auto ev_track = storage->get_data<event_track>("trackkalmanhit");
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
    auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
    auto ev_mcphotons = storage->get_data<event_simphotons>("mcreco");



    if (!_use_mc && !_photon_bomb) {
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

	  pt.t = pt.x/(det_width/det_drift_time) ; // this is an upper bound on the drift time

          tpc_obj.emplace_back(pt);
          tpc_obj.idx = n;
        }

        _mgr.Emplace(std::move(tpc_obj));
      }
    }
    // use MC tracks
    else if (_use_mc && !_photon_bomb) {

      if (!ev_mctrack || ev_mctrack->empty()) 
	{
	  return false;
	}
      for (size_t n = 0; n < ev_mctrack->size(); n++) {

        auto const& trk = ev_mctrack->at(n);

        if (trk.size() > 2) {

          // per track calculate the shift in x-direction
          // so that the x-position is what would be seen
          // in the TPC, not the truth x-position
          double event_time = trk[0].T(); // ns
          double shift_x = event_time * (det_width / det_drift_time);

          _trk_time = trk[0].T() / 1000.;
          _trk_x    = trk[0].X();
          _trk_shift = shift_x;
          _trk_min_x = 1036.;
          _trk_max_x = -1036.;
          for (size_t i = 0; i < trk.size(); ++i) {
            if (trk[i].X() > _trk_max_x) { _trk_max_x = trk[i].X(); }
            if (trk[i].X() < _trk_min_x) { _trk_min_x = trk[i].X(); }
          }

          _track_tree->Fill();
	}
      }

      // Get MC QCluster list
      // Fills _qcluster_v inside MCQCluster
      _interaction_algo.Construct(*ev_mctrack,*ev_mcshower);
      
      std::vector<flashana::QCluster_t> qcluster_v;
      std::vector<flashana::MCSource_t> source_v;
      // Fills qcluster_v with the content of _qcluster_v which is filled above in Construct()
      _interaction_algo.Swap(std::move(qcluster_v),std::move(source_v));

      for(auto& qcluster : qcluster_v){
        if( qcluster.idx != -1)
	  for (size_t ii = 0;  ii<qcluster.size(); ++ii)
	    {
	      qcluster[ii].t = qcluster[ii].x/(det_width/det_drift_time) ; // this is drift time, which will create an upper bound on where track could have been in x; will enforce the bound later in PhotonLibHypothesis::FillEstimate().
	    }
	  _mgr.Emplace(std::move(qcluster));
	  if ( _photlib_tree_config == "seen" )
	    {
	      auto const& qcc(qcluster);
	      Fill_PVL_Tree(qcc);
	    }

	}

      for(auto const& s : source_v) {

	if(s.g4_time == ::flashana::kINVALID_DOUBLE) {
	  print(msg::kWARNING,__FUNCTION__,Form("Found unknown QCluster MC source (G4 T=%g)",s.g4_time));
	  continue;
	  }

	//  Need to check that the qcluster we're dealing with has greater than
	//  2 energy dep points.  
	bool good = true;
	for(auto const& q : qcluster_v){
	  n_int++;
	  if(q.idx == s.index_id){
	    if(q.size() < 2 || q.idx == -1){
	      good = false;
	      break;
	      }
	    }
	  }
       
        if( good ){
	_t0 = s.g4_time;
	_n_pe = 0;
	_int_e = s.energy_deposit;
	
	for (auto const & h : *ev_hit ) {
	  if (h.PeakTime() > (_t0 / 1000. - 10.) && h.PeakTime() < (_t0 / 1000. + 10.))
	    _n_pe += h.PE();
	   } // end looping over all hits for this interaction
	
	_int_tree->Fill();
	}
      } // end looping over all MC source
    }//else if _use_mc
    else if (_photon_bomb) {

      n_int++;
      flashana::QPoint_t qpt(_x_pb,_y_pb,_z_pb,_nphoton_pb);
      flashana::QCluster_t qcluster;
      qcluster.emplace_back(qpt);
      _interaction_algo.Construct(qcluster);

      //      for(auto& qcluster : qcluster_v){
      //        if( qcluster.idx != -1)
      _mgr.Emplace(std::move(qcluster));
      //	}

	_t0 = 0.0;
	_n_pe = _nphoton_pb;
	_int_e = _n_pe/29000.; // MeV
	
	_int_tree->Fill();

    }//else if photon bomb

    for (size_t n = 0; n < ev_flash->size(); n++) {

      auto const& flash = ev_flash->at(n);

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

    /*
    for(auto const& calo : *ev_calo) {

      ::flashana::TPCObject_t tpc_obj;

      auto const& dedx = calo.dEdx();
      auto const& dx   = calo.TrkPitchVec();

      std::cout<<dedx.size()<<" : "<<dx.size()<<std::endl;

    }
    */
    auto const res = _mgr.Match();

    std::cout << "UBT0Finder::analyze(): Number of flashes, qclusters, matches is " << ev_flash->size() << ", " << _interaction_algo.QClusters().size() << ", " << res.size() << std::endl;
    
    ::geoalgo::LineSegment line;
    ::geoalgo::Point_t pt(0, 0, 0);
    ::geoalgo::GeoAlgo geoalg;
    for (auto const& match : res) {
      auto const& flash = (*ev_flash)[match.flash_id];
      _flash_y = flash.YCenter();
      _flash_z = flash.ZCenter();
      _tpc_x   = match.tpc_point.x;
      _tpc_y   = match.tpc_point.y;
      _tpc_z   = match.tpc_point.z;
      _tpc_t   = match.tpc_point.t;
      _npe     = flash.TotalPE();
      _score   = match.score;
      _flash_time = flash.Time();
      _mc_time = _mc_x = _mc_y = _mc_z = -1;

      if (_use_mc || _photon_bomb) {

//	std::cout<<"Match things...: "<<match.tpc_id<<", and size of ev_mctrk : "<<ev_mctrack->size()<<std::endl ;

	if (_use_mc)
	  {
	    auto const& mct = (*ev_mctrack)[match.tpc_id];
	    _mc_time = mct[0].T() * 1.e-3;
	    _mc_x = mct[0].X();
	    _mc_y = mct[0].Y();
	    _mc_z = mct[0].Z();

	    if(_mc_time < -2050 || _mc_time > 2750)
	      continue;

	    double event_time = mct[0].T(); // ns
	    if ( mct[0].E() - mct[mct.size() - 1].E() > _e_diff )
	      _time_diff->Fill(1000 * (_flash_time - _mc_time)); //
	    _trk_shift = event_time * (det_width / det_drift_time);

	    _mc_length = std::sqrt(std::pow(mct.Start().X()-mct.End().X(),2.)+std::pow(mct.Start().Y()-mct.End().Y(),2.)+std::pow(mct.Start().Z()-mct.End().Z(),2.));

	    _mc_trajx.clear(); _mc_trajy.clear(); _mc_trajz.clear();
	    _mc_trajx.reserve(mct.SegmentCenter().size());
	    _mc_trajy.reserve(mct.SegmentCenter().size());
	    _mc_trajz.reserve(mct.SegmentCenter().size());
	    for ( auto const& pt :  mct.SegmentCenter())
	      {
		_mc_trajx.emplace_back(pt[0]);
		_mc_trajy.emplace_back(pt[1]);
		_mc_trajz.emplace_back(pt[2]);
	      }
		
	  }
	else if (_photon_bomb)
	      _time_diff->Fill(1000 * (_flash_time - 0 )); //


	//        double min_dist = 1e12;
        pt[0] = _tpc_x;
        pt[1] = _tpc_y;
        pt[2] = _tpc_z;

        //double min_x = 1e9;
        //double max_x = 0;
        //for (size_t i = 0; i < mct.size() - 1; ++i) {
        //  auto const& step1 = mct[i];
        //  auto const& step2 = mct[i + 1];
	//  std::cout<<"Step x y z: "<<step2.X()<<", "<<step2.Y()<<", "<<step2.Z()<<std::endl ;
        //  line.Start(step1.X(), step1.Y(), step1.Z());
        //  line.End(step2.X(), step2.Y(), step2.Z());
        //  auto const closest_pt = geoalg.ClosestPt(line, pt);
        //  double dist = closest_pt.SqDist(pt);
        //  if (dist < min_dist) {
        //    min_dist = dist;
        //    _mc_x = closest_pt[0];
        //    _mc_y = closest_pt[1];
        //    _mc_z = closest_pt[2];
        //  }

        //  if (step1.X() < min_x) min_x = step1.X();
        //  if (step1.X() > max_x) max_x = step1.X();
        //}
        //_mc_dx = max_x - min_x;

      }

      _tpcID = match.tpc_id;
      _event = storage->event_id();
      _flashmatch_tree->Fill();

    }// for all matches


    // make an entry of the flash-matching results for every MCTrack
    if (!_use_mc)
      return true;

    _npts = 0 ;
    for (size_t n = 0; n < ev_mctrack->size(); n++) {
      auto const& mct = ev_mctrack->at(n);
      // ignore tracks with < 2 steps
      if (mct.size() <= 2) continue;
      // find the flash that was matched for this MCTrack (if any)
      _mc_time = mct[0].T() * 1.e-3;
      _mc_edep = mct[0].E() - mct[mct.size() - 1].E();
      double event_time = mct[0].T(); // ns
      double det_drift_time = 2.2E6; // ns
      double det_width = 256.; // cm
      _trk_shift = event_time * (det_width / det_drift_time);
      _trk_min_x =  1036.;
      _trk_max_x = -1036.;
      _trk_min_abs_x =  2036.;
      _trk_max_abs_x = -2036.;

      for (size_t i = 0; i < (mct.size() - 1); ++i) {

        auto const& pt1 = mct[i].Position();
        auto const& pt2 = mct[i + 1].Position();

        if ( (pt1[0] + _trk_shift) > _trk_max_abs_x) _trk_max_abs_x = pt1[0] + _trk_shift;
        if ( (pt1[0] + _trk_shift) < _trk_min_abs_x) _trk_min_abs_x = pt1[0] + _trk_shift;

        double dx = pt2[0] - pt1[0];
        double x  = pt1[0] + dx / 2. + _trk_shift;

        if (x > _trk_max_x) _trk_max_x = x;
        if (x < _trk_min_x) _trk_min_x = x;
      }

      _matched    = 0 ;
      _npe        = 0;
      _npts = mct.size() ;

      for (auto const& match : res) {
        if (match.tpc_id == n) {
          _matched = 1;
          auto const& flash = (*ev_flash)[match.flash_id];
          _flash_time  = flash.Time();
          _npe = flash.TotalPE();
        }
      }

      _eff_tree->Fill();
    }// for all MCTracks

    _nflash_v_nint->Fill(n_int, n_flash);

    return true;
  }

  bool UBT0Finder::finalize() {
    if (_fout) {


      _fout->cd();
      _time_diff->GetXaxis()->SetTitle("Delta T [ns]" );

      auto eligible_matches = _int_tree->GetEntries("_t0>-2050000 && _t0 < 2750000") ;

      std::cout << "\nEfficiency (#matches/#interactions)  : " << float(_flashmatch_tree->GetEntries()) / eligible_matches * 100 << "%, (" << _flashmatch_tree->GetEntries() << "/" << eligible_matches << ")" << std::endl;
      std::cout << "Correctness (#good matches/#matches) : " << _time_diff->Integral(8, 24) / _flashmatch_tree->GetEntries() * 100 << "%, (" << _time_diff->Integral(8, 24) << "/" << _flashmatch_tree->GetEntries() << ")" << std::endl;


      _flashmatch_tree->Write();
      _int_tree->Write();
      _track_tree->Write();
      _eff_tree->Write();
      _flash_tree->Write();
      _nflash_v_nint->Write();
      _time_diff->Write();
      if (_photlib_tree_config!=NULL)
	_photlib_tree->Write();

    }
    return true;
  }

}
#endif
