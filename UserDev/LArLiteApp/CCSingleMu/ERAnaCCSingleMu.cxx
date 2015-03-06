#ifndef ERTOOL_ERANACCSINGLEMU_CXX
#define ERTOOL_ERANACCSINGLEMU_CXX

#include "ERAnaCCSingleMu.h"
#include "GeoAlgo/GeoAlgo.h"
namespace ertool {

  ERAnaCCSingleMu::ERAnaCCSingleMu() : AnaBase()
  {
    _name     = "ERAnaCCSingleMu";
    _primary_cut_dist = 5;
    _primary_range    = 5;
    _min_trk_length   = 10;
    _use_mc = false;
  }

  void ERAnaCCSingleMu::Reset()
  {}

  void ERAnaCCSingleMu::ProcessBegin()
  {
    hPrimaryCtr = new TH1D("hPrimaryCtr","Number of possible primary trajectories; Count; Events",
			   100,-0.5,99.5);
    hPrimaryPID = new TH1D("hPrimaryPID","Primary Track PID",
			   5,-0.5,4.5);
    hPrimaryPID->GetXaxis()->SetBinLabel(1,"unknown");
    hPrimaryPID->GetXaxis()->SetBinLabel(2,"p");
    hPrimaryPID->GetXaxis()->SetBinLabel(3,"K");
    hPrimaryPID->GetXaxis()->SetBinLabel(4,"pion");
    hPrimaryPID->GetXaxis()->SetBinLabel(5,"mu");
    hPrimaryPID->GetXaxis()->SetNdivisions(5,kFALSE);
    hPrimaryPID->GetXaxis()->SetTicks("-");
    //hPrimaryPID->GetXaxis()->LabelsOption("v");
    
  }

  bool ERAnaCCSingleMu::Analyze(const EventData &data, const ParticleSet &ps)
  { 

    if(_use_mc) {
      auto const& mc_data = MCEventData();
      auto const& mc_ps = MCParticleSet();
      return AnalyzeEventData(mc_data,mc_ps);
    }else
      return AnalyzeEventData(data,ps);

  }

  bool ERAnaCCSingleMu::AnalyzeEventData(const EventData &data, const ParticleSet& ps)
  {
    
    ::geoalgo::GeoAlgo geo_alg;
    std::vector<bool> primary_flag(data.AllTrack().size(),false);
    for(auto const& trk : data.Track())
      //Accept if it has some length
      if(trk->Length() > _min_trk_length) primary_flag[trk->ID()] = true;

    // Find primary candidate tracks
    if(primary_flag.size()>1) {
      for(auto const& pair : data.TrackCombination(2)) {

	auto const& trk1 = pair[0];
	auto const& trk2 = pair[1];

	// First check if two tracks have start points close enough
	double dist0 = trk1->front().Dist(trk2->front());
	if(dist0 < _primary_range) continue;

	// Is trk1 from trk2?
	if(primary_flag[trk1->ID()]) {
	  double dist1 = geo_alg.SqDist(trk1->front(),*trk2);
	  if(dist1 < _primary_cut_dist) 
	    primary_flag[trk1->ID()]=false;	  
	}
	
	// Is trk2 from trk1?
	if(primary_flag[trk2->ID()]) {
	  double dist2 = geo_alg.SqDist(trk2->front(),*trk1);
	  if(dist2 < _primary_cut_dist) primary_flag[trk2->ID()]=false;
	}
      }
    }
    
    double ctr=0;
    for(auto const& v : primary_flag) if(v) ctr++;
    hPrimaryCtr->Fill(ctr);
    
    for(size_t i=0; i<primary_flag.size(); ++i) {
      if(!primary_flag[i]) continue;
      hPrimaryPID->Fill(data.Track(i)._pid);
    }

    return true; 
  }

  void ERAnaCCSingleMu::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      hPrimaryCtr->Write();
      hPrimaryPID->Write();
    }
  }

}

#endif
