#ifndef ERTOOL_ERANACCSINGLEMU_CXX
#define ERTOOL_ERANACCSINGLEMU_CXX

#include "ERAnaCCSingleMu.h"
#include "GeoAlgo/GeoAlgo.h"
namespace ertool {

  ERAnaCCSingleMu::ERAnaCCSingleMu() : AnaBase()
  {
    _name   = "ERAnaCCSingleMu";
    _use_mc = true;
    hMCPrimaryCtr = nullptr;
    hMCPrimaryPID = nullptr;
    hRecoPrimaryCtr = nullptr;
    hRecoPrimaryPID = nullptr;
  }

  void ERAnaCCSingleMu::Reset()
  {}

  void ERAnaCCSingleMu::ProcessBegin()
  {
    if(!hMCPrimaryCtr)
      hMCPrimaryCtr = new TH1D("hMCPrimaryCtr","Number of possible primary trajectories; Count; Events",
			       100,-0.5,99.5);
    hMCPrimaryCtr->Reset();
    
    if(!hMCPrimaryPID) {
      hMCPrimaryPID = new TH1D("hMCPrimaryPID","Primary Track PID",
			       5,-0.5,4.5);
      hMCPrimaryPID->GetXaxis()->SetBinLabel(1,"unknown");
      hMCPrimaryPID->GetXaxis()->SetBinLabel(2,"p");
      hMCPrimaryPID->GetXaxis()->SetBinLabel(3,"K");
      hMCPrimaryPID->GetXaxis()->SetBinLabel(4,"pion");
      hMCPrimaryPID->GetXaxis()->SetBinLabel(5,"mu");
      hMCPrimaryPID->GetXaxis()->SetNdivisions(5,kFALSE);
      hMCPrimaryPID->GetXaxis()->SetTicks("-");
    }
    hMCPrimaryPID->Reset();

    if(!hRecoPrimaryCtr)
      hRecoPrimaryCtr = new TH1D("hRecoPrimaryCtr","Number of possible primary trajectories; Count; Events",
				 100,-0.5,99.5);
    hRecoPrimaryCtr->Reset();

    if(!hRecoPrimaryPID) {
      hRecoPrimaryPID = new TH1D("hRecoPrimaryPID","Primary Track PID",
				 5,-0.5,4.5);
      hRecoPrimaryPID->GetXaxis()->SetBinLabel(1,"unknown");
      hRecoPrimaryPID->GetXaxis()->SetBinLabel(2,"p");
      hRecoPrimaryPID->GetXaxis()->SetBinLabel(3,"K");
      hRecoPrimaryPID->GetXaxis()->SetBinLabel(4,"pion");
      hRecoPrimaryPID->GetXaxis()->SetBinLabel(5,"mu");
      hRecoPrimaryPID->GetXaxis()->SetNdivisions(5,kFALSE);
      hRecoPrimaryPID->GetXaxis()->SetTicks("-");
    }
    hRecoPrimaryPID->Reset();
  }

  bool ERAnaCCSingleMu::Analyze(const EventData &data, const ParticleSet &ps)
  { 

    // Analyze selected sample
    size_t reco_primary_ctr=0;
    for(auto const& p : ps) {
      if(p.Type() != Particle::kTrack) continue;
      auto const& trk = data.Track(p.RecoObjID());
      hRecoPrimaryPID->Fill(trk._pid);
      reco_primary_ctr++;
    }
    hRecoPrimaryCtr->Fill(reco_primary_ctr);

    // Analyze MC
    if(_use_mc) {
      size_t mc_primary_ctr=0;
      auto const& mc_data = MCEventData();
      auto const& mc_ps = MCParticleSet();
      for(auto const& origin_p : mc_ps) {

	for(auto const& p : origin_p.Daughters()) {
	  if(p.Type() != Particle::kTrack) continue;
	  auto const& trk = mc_data.Track(p.RecoObjID());
	  hMCPrimaryPID->Fill(trk._pid);
	  mc_primary_ctr++;
	}
      }
      hMCPrimaryCtr->Fill(mc_primary_ctr);
    }

    return true;
  }

  void ERAnaCCSingleMu::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      hMCPrimaryCtr->Write();
      hMCPrimaryPID->Write();
      hRecoPrimaryCtr->Write();
      hRecoPrimaryPID->Write();
    }
  }

}

#endif
