#ifndef LARLITE_PADDLEUBT0FINDER_CXX
#define LARLITE_PADDLEUBT0FINDER_CXX

#include "PaddleUBT0Finder.h"

namespace larlite {

  PaddleUBT0Finder::PaddleUBT0Finder()
    : _int_tree(nullptr)
    , _track_tree(nullptr)
    , _flashmatch_tree(nullptr)
    , _eff_tree(nullptr)
    , _nflash_v_nint(nullptr)
    , _time_diff(nullptr)
  {
    _name="PaddleUBT0Finder";
    _fout=0;
    
    _e_diff    = 10;
    UseAbsolutePE(false);
    SetStepLength(0.5) ;
  }
  
  bool PaddleUBT0Finder::initialize() {
    
    _time_diff = new TH1D("time_diff","Matched Flash vs. MCTrack",100,0,500);
    
    _nflash_v_nint = new TH2D("_nflash_v_nint","OpFlash with PE > 10 vs Nint/event ",50,0,50,30,0,30);
    
    _int_tree = new TTree("int_tree","");
    _int_tree->Branch("_t0",&_t0,"t0/D");
    _int_tree->Branch("_n_pe",&_n_pe,"n_pe/D");
    _int_tree->Branch("_int_e",&_int_e,"int_e/D");
    _int_tree->Branch("_n_flash",&_n_flash,"n_flash/D");
    
    _track_tree = new TTree("track_tree","");
    _track_tree->Branch("trk_time",&_trk_time,"trk_time/D");
    _track_tree->Branch("trk_max_x",&_trk_max_x,"trk_max_x/D");
    _track_tree->Branch("trk_min_x",&_trk_min_x,"trk_min_x/D");
    _track_tree->Branch("trk_shift",&_trk_shift,"trk_shift/D");
    _track_tree->Branch("trk_x",&_trk_x,"trk_x/D");
    
    _flashmatch_tree = new TTree("flashmatch_tree","");
    _flashmatch_tree->Branch("npe",&_npe,"npe/D");
    _flashmatch_tree->Branch("fy",&_flash_y,"fy/D");
    _flashmatch_tree->Branch("fz",&_flash_z,"fz/D");
    _flashmatch_tree->Branch("tx",&_tpc_x,"tx/D");
    _flashmatch_tree->Branch("ty",&_tpc_y,"ty/D");
    _flashmatch_tree->Branch("tz",&_tpc_z,"tz/D");
    _flashmatch_tree->Branch("ft",&_flash_time,"ft/D");
    _flashmatch_tree->Branch("mct",&_mc_time,"mct/D");
    _flashmatch_tree->Branch("mcE",&_mc_edep,"mcE/D");
    _flashmatch_tree->Branch("mcx",&_mc_x,"mcx/D");
    _flashmatch_tree->Branch("mcy",&_mc_y,"mcy/D");
    _flashmatch_tree->Branch("mcz",&_mc_z,"mcz/D");
    _flashmatch_tree->Branch("mc_dx",&_mc_dx,"mc_dx/D");
    _flashmatch_tree->Branch("score",&_score,"score/D");
    _flashmatch_tree->Branch("trk_shift",&_trk_shift,"trk_shift/D");
    
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
  
  bool PaddleUBT0Finder::analyze(storage_manager* storage) {
    ////////////////////////////////////////////////////
    
    _mgr.Reset();
    const ::larutil::Geometry* g = ::larutil::Geometry::GetME();

    auto ev_flash = storage->get_data<event_opflash>("satOpFlash");// opflash");
    auto ev_hit= storage->get_data<event_ophit>("satOpFlash");// opflash");
    
    auto ev_track = storage->get_data<event_track>("trackkalmanhit");
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");
    
    
    if(!ev_flash || ev_flash->empty()) {
      std::cout<<"No opflash found. Skipping event: "<<storage->event_id()<<std::endl;
      return false;
    }

    // For TH2D-- number of flash per event > x PE
    // Number interactions per event > y MeV
    int n_flash = 0;
    int n_int = 0;
    
    ::flashana::LightPath LP;
    
    for ( auto & f : *ev_flash) {
      if (f.TotalPE() > 10 )
	n_flash++;
    }
    
    
    ////////////////////////////////////////////////////
    
    return true;
  }

  bool PaddleUBT0Finder::finalize() {

  
    return true;
  }

}
#endif
