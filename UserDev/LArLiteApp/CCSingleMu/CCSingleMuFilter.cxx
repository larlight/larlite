#ifndef CCSINGLEMUFILTER_CXX
#define CCSINGLEMUFILTER_CXX

#include "CCSingleMuFilter.h"

namespace larlite {

  CCSingleMuFilter::CCSingleMuFilter()
    : hMuEnergy(nullptr)
    , hMuCount(nullptr)
  {
  }

  bool CCSingleMuFilter::initialize() {

    hMuEnergy = new TH1D("hMuEnergy","Muon Energy; Energy [MeV]; Muon Counts",
			 100,0,2000);
    hMuCount  = new TH1D("hMuCount","Muon Count; Count; Events",
			 10,-0.5,9.5);
    return false;
  }
  
  bool CCSingleMuFilter::analyze(storage_manager* storage) {

    auto ev_mci = storage->get_data<event_mctruth>("generator");

    if(!ev_mci || ev_mci->size() < 0) {
      print(msg::kERROR,__FUNCTION__,"MCTruth not found...");
      throw std::exception();
    }  

    int numu_count   = 0;
    int numu_trackid = -1;
    int mu_count = 0;
    for(auto const& mci : *ev_mci) {

      // Look for numu
      for(auto const& mcp : mci.GetParticles()) {
	if(mcp.StatusCode() == 0 && mcp.PdgCode() == 14) {
	  numu_trackid = mcp.TrackId();
	  break;
	}
      }

      if(numu_trackid<0) continue;
      // Look for a daughter muon
      for(auto const& mcp : mci.GetParticles()) {

	if( mcp.StatusCode() == 1 && 
	    mcp.PdgCode() == 13   && 
	    mcp.Mother() == numu_trackid ) {
	  ++mu_count;
	  hMuEnergy->Fill(mcp.Trajectory()[0].Momentum().E());
	}
      }
    }

    hMuCount->Fill(mu_count);
    // Check if there's any numu & daughter muon found. If not, return false
    if(!mu_count) return false;

    return true;
  }

  bool CCSingleMuFilter::finalize() {

    return true;
  }

}
#endif
