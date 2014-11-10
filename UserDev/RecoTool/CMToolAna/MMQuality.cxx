#ifndef RECOTOOL_MMQUALITY_CXX
#define RECOTOOL_MMQUALITY_CXX

#include "MMQuality.h"

namespace larlite {
  MMQuality::MMQuality() {
    
    _name="MMQuality"; 
    _fout=0;

    fClusterProducer = "";
    fShowerProducer  = "";
    
    vShowerClusterEff.clear();
    vShowerClusterPur.clear();
    hMatchCorrectness = nullptr;

  }

  bool MMQuality::initialize() {

    if(fClusterProducer.empty() && fShowerProducer.empty()) {

      print(msg::kERROR,__FUNCTION__,
	    "Cluster & Shower producer's name is not set!");
      return false;

    }

    if(!(fClusterProducer.empty()) && !(fShowerProducer.empty())) {

      print(msg::kERROR,__FUNCTION__,
	    Form("Shower producer (\"%s\") and Cluster producer (\"%s\") both set! (can only set one of two)",
		 fShowerProducer.c_str(),
		 fClusterProducer.c_str())
	    );
      return false;
    }

    auto geo = larutil::Geometry::GetME();

    //
    // Cluster purity, efficiency histograms initialization
    //
    for(auto& h : vShowerClusterEff) delete h;
    for(auto& h : vShowerClusterPur) delete h;

    vShowerClusterEff.clear();
    vShowerClusterPur.clear();
    for(size_t plane=0; plane<geo->Nplanes(); ++plane) {

      vShowerClusterEff.push_back(new TH1D(Form("hShowerClusterEff_Plane%zu",plane),
					   "Best Shower Cluster's Charge Efficiency; Efficiency; Clusters",
					   101,-0.005,1.005));

      vShowerClusterPur.push_back(new TH1D(Form("hShowerClusterPur_Plane%zu",plane),
					   "Best Shower Cluster's Charge Purity; Purity; Clusters",
					   101,-0.005,1.005));
    }

    //
    // Matching correctness histogram initialization
    //
    if(hMatchCorrectness) delete hMatchCorrectness;
    hMatchCorrectness = new TH1D("hMatchCorrectness",
				 "Shower 2D Cluster Matching Correctness; Correctness; Showers",
				 101,-0.005,1.005);

    return true;
  }
  
  bool MMQuality::analyze(storage_manager* storage) {

    if(fClusterProducer.empty()) {

      auto ev_shower = storage->get_data<event_shower>(fShowerProducer);

      if(!ev_shower) {
	print(msg::kERROR,__FUNCTION__,
	      Form("Did not find shower produced by \"%s\"",fShowerProducer.c_str())
	      );
	return false;
      }

      auto ass_keys = ev_shower->association_keys(data::kCluster);

      if(!(ass_keys.size())) {
	print(msg::kERROR,__FUNCTION__,
	      Form("No associated cluster found to a shower produced by \"%s\"",
		   fShowerProducer.c_str())
	      );
      }

      fClusterProducer = ass_keys[0];
    }

    if(!fBTAlg.BuildMap(storage, fClusterProducer)) {
      print(msg::kERROR,__FUNCTION__,"Failed to build back-tracking map for MC...");
      return false;
    }

    auto mcshower_index_v = fBTAlg.BTAlg().UniqueShowerID();

    auto geo = larutil::Geometry::GetME();

    // Fill cluster quality plots
    for(auto const& mcs_index : mcshower_index_v) {

      for(size_t plane = 0; plane < geo->Nplanes(); ++plane) {

	auto const ep = fBTAlg.BestClusterEP(mcs_index,plane);

	if( ep.first==0 && ep.second==0 ) continue;

	vShowerClusterEff[plane]->Fill(ep.first);
	vShowerClusterPur[plane]->Fill(ep.second);

      }

    }

    // If this is not for 3D shower comparison, return
    if(fShowerProducer.empty()) return true;

    // Retrieve shower data product
    auto ev_shower = storage->get_data<event_shower>(fShowerProducer);
    
    // Retrieve associated cluster indices
    auto ass_cluster_v = ev_shower->association(data::kCluster,fClusterProducer);

    for(size_t shower_index=0; shower_index < ass_cluster_v.size(); ++shower_index) {

      auto res = fBTAlg.ShowerCorrectness(ass_cluster_v[shower_index]);

      //
      // Fill histogram
      //
      hMatchCorrectness->Fill(res.second);

    }
  
    return true;
  }

  bool MMQuality::finalize() {

    if(_fout) {

      // Write cluster histograms if any entry made
      if(vShowerClusterEff.size() && vShowerClusterEff[0]->GetEntries()) {

	for(auto& h : vShowerClusterEff) h->Write();
	for(auto& h : vShowerClusterPur) h->Write();

      }

      // Write shower histograms if any entry made
      if(hMatchCorrectness->GetEntries())
	
	hMatchCorrectness->Write();
    }

    return true;
  }

}
#endif
