#ifndef CCSINGLEMUFILTER_CXX
#define CCSINGLEMUFILTER_CXX

#include "CCSingleMuFilter.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mctrack.h"
namespace larlite {

  CCSingleMuFilter::CCSingleMuFilter()
    : hPrimaryMuEnergy(nullptr)
    , hPrimaryMuCount(nullptr)
    , hAllMuEnergy(nullptr)
    , hAllMuCount(nullptr)
    , hPrimaryPiEnergy(nullptr)
    , hPrimaryPiCount(nullptr)
    , hAllPiEnergy(nullptr)
    , hAllPiCount(nullptr)
  {
    _name = "CCSingleMuFilter";
    _flip = false;
  }

  bool CCSingleMuFilter::initialize() {

    hPrimaryMuEnergy = new TH1D("hPrimaryMuEnergy","Muon Energy; Energy [MeV]; Muon Counts",
				100,0,2000);
    hPrimaryMuCount  = new TH1D("hPrimaryMuCount","Muon Count; Count; Events",
				10,-0.5,9.5);
    hAllMuEnergy = new TH1D("hAllMuEnergy","Muon Energy; Energy [MeV]; Muon Counts",
			    100,0,2000);
    hAllMuCount  = new TH1D("hAllMuCount","Muon Count; Count; Events",
			    10,-0.5,9.5);

    hPrimaryPiEnergy = new TH1D("hPrimaryPiEnergy","Pion Energy; Energy [MeV]; Pion Counts",
				100,0,2000);
    hPrimaryPiCount  = new TH1D("hPrimaryPiCount","Pion Count; Count; Events",
				10,-0.5,9.5);
    hAllPiEnergy = new TH1D("hAllPiEnergy","Pion Energy; Energy [MeV]; Pion Counts",
			    100,0,2000);
    hAllPiCount  = new TH1D("hAllPiCount","Pion Count; Count; Events",
			    10,-0.5,9.5);
    return true;
  }
  
  bool CCSingleMuFilter::analyze(storage_manager* storage) {

    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    if(!ev_mct){
      print(msg::kERROR,__FUNCTION__,"MCTrack not found...");
      throw std::exception();
    }  

    int primary_mu_count = 0;
    int primary_pi_count = 0;
    int all_mu_count = 0;
    int all_pi_count = 0;

    // Search for muons in MCTrack
    for(auto const& mct : *ev_mct) {
      
      bool muon = (mct.PdgCode() == 13 || mct.PdgCode() == -13 );
      bool pion = (mct.PdgCode() == 211 || mct.PdgCode() == -211);

      if(!muon && !pion) continue;

      auto energy = mct.back().E() - mct.front().E();

      if(muon) {
	hAllMuEnergy->Fill(energy);
	all_mu_count++;
      }
      if(pion) {
	hAllPiEnergy->Fill(energy);
	all_pi_count++;
      }
      
      if(mct.TrackID() == mct.AncestorTrackID() &&
	 mct.Origin() == simb::kBeamNeutrino ) {

	if(muon) {
	  hPrimaryMuEnergy->Fill(energy);
	  primary_mu_count++;
	}
	if(pion) {
	  hPrimaryPiEnergy->Fill(energy);
	  primary_pi_count++;
	}
      }
    }

    hPrimaryMuCount->Fill(primary_mu_count);
    hPrimaryPiCount->Fill(primary_pi_count);
    hAllMuCount->Fill(all_mu_count);
    hAllPiCount->Fill(all_pi_count);

    if(_flip) return (primary_mu_count != 1);
    else return (primary_mu_count == 1);
  }

  bool CCSingleMuFilter::finalize() {

    if(_fout) {
      _fout->cd();
      hPrimaryMuCount->Write();
      hPrimaryMuEnergy->Write();
      hAllMuCount->Write();
      hAllMuEnergy->Write();
      hPrimaryPiCount->Write();
      hPrimaryPiEnergy->Write();
      hAllPiCount->Write();
      hAllPiEnergy->Write();
    }

    return true;
  }

}
#endif
