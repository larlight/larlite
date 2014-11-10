#ifndef SHOWERQUALITY_CXX
#define SHOWERQUALITY_CXX

#include "ShowerQuality.h"

namespace larlite {
  ShowerQuality::ShowerQuality() {
    
    _name="ShowerQuality"; 
    _fout=0;

    fShowerProducer  = "";
    
    hMatchCorrectness = nullptr;

    hVtxDX = nullptr;
    hVtxDY = nullptr;
    hVtxDZ = nullptr;
    hVtxDR = nullptr;

    hDCosX = nullptr;
    hDCosY = nullptr;
    hDCosZ = nullptr;
    h3DAngleDiff = nullptr;

    hEnergyCorr  = nullptr;
    hEnergyAssym = nullptr;
    hEnergyDiff  = nullptr;

    hMatchedClusterPur = nullptr;
    hMatchedClusterEff = nullptr;

    mDEDX.clear();
    hBestPlane = nullptr;
  }

  bool ShowerQuality::initialize() {

    if(fShowerProducer.empty()) {

      print(msg::kERROR,__FUNCTION__,
	    "Shower producer's name is not set!");
      return false;

    }

    auto geo = larutil::Geometry::GetME();

    //
    // Matching correctness histogram initialization
    //
    if(hMatchCorrectness) delete hMatchCorrectness;
    hMatchCorrectness = new TH1D("hMatchCorrectness",
				 "Shower 2D Cluster Matching Correctness; Correctness; Showers",
				 101,-0.005,1.005);

    //
    // 3D Vtx (start point) MC/Reco comparison histogram initialization
    //
    if(hVtxDX) delete hVtxDX;
    if(hVtxDY) delete hVtxDY;
    if(hVtxDZ) delete hVtxDZ;
    if(hVtxDR) delete hVtxDR;

    hVtxDX = new TH1D("hVtxDX",
		      "Reco - MC Start X [cm] Displacement; #DeltaX [cm]; Showers",
		      200,-100,100);

    hVtxDY = new TH1D("hVtxDY",
		      "Reco - MC Start Y [cm] Displacement; #DeltaY [cm]; Showers",
		      200,-100,100);

    hVtxDZ = new TH1D("hVtxDZ",
		      "Reco - MC Start Z [cm] Displacement; #DeltaZ [cm]; Showers",
		      200,-100,100);

    hVtxDR = new TH1D("hVtxDR",
		      "Reco - MC Start 3D Vtx Displacement; #DeltaR [cm]; Showers",
		      200,-100,100);

    //
    // 3D Angular MC/Reco comparison histogram initialization
    //
    if(hDCosX) delete hDCosX;
    if(hDCosY) delete hDCosY;
    if(hDCosZ) delete hDCosZ;
    if(h3DAngleDiff) delete h3DAngleDiff;

    hDCosX = new TH1D("hDCosX",
		      "Direction Unit Vector Reco - MC #DeltaX; #DeltaCosX; Showers",
		      100,-2,2);

    hDCosY = new TH1D("hDCosY",
		      "Direction Unit Vector Reco - MC #DeltaY; #DeltaCosY; Showers",
		      100,-2,2);

    hDCosZ = new TH1D("hDCosZ",
		      "Direction Unit Vector Reco - MC #DeltaZ; #DeltaCosZ; Showers",
		      100,-2,2);

    h3DAngleDiff = new TH1D("h3DAngleDiff",
			    "3D Opening Angle Between Reco & MC; Opening Angle [degrees]; Showers",
			    181,-0.5,180.5);

    //
    // Energy MC/Reco comparison histogram initialization
    //
    if(hEnergyCorr)  delete hEnergyCorr;
    if(hEnergyAssym) delete hEnergyAssym;
    if(hEnergyDiff)  delete hEnergyDiff;

    hEnergyCorr = new TH2D("hEnergyCorr",
			   "Reco (x) vs. MC (y) Energy Comparison; Reco Energy [MeV]; MC Energy [MeV]",
			   200,0,1000,200,0,1000);

    hEnergyAssym = new TH1D("hEnergyAssym",
			    "MC - Reco Energy Fractional Difference; Assymetry; Showers",
			    201,-1.005,1.005);
    
    hEnergyDiff = new TH1D("hEnergyDiff",
			   "MC - Reco Energy Difference; Energy Difference [MeV]; Showers",
			   200,0,1000);

    //
    // Shower cluster purity & efficiency histograms initialization
    //
    if(hMatchedClusterEff) delete hMatchedClusterEff;
    if(hMatchedClusterPur) delete hMatchedClusterPur;
    
    hMatchedClusterEff = new TH1D("hMatchedClusterEff",
				  "Matched Shower Cluster's Charge Efficiency; Efficiency; Clusters",
				  101,-0.005,1.005);

    hMatchedClusterPur = new TH1D("hMatchedClusterPur_Plane%zu",
				  "Matched Shower Cluster's Charge Purity; Purity; Clusters",
				  101,-0.005,1.005);


    //
    // Best plane ID histogram initialization
    //
    hBestPlane = new TH1D("hBestPlane",
			  "Best Plane (for energy & dE/dx estimate); Plane ID; Showers",
			  geo->Nplanes(),-0.5,geo->Nplanes()-0.5);

    InitializeAnaTree();
    
    return true;
  }
  
  bool ShowerQuality::analyze(storage_manager* storage) {

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
    
    if(!fBTAlg.BuildMap(storage, ass_keys[0])) {
      print(msg::kERROR,__FUNCTION__,"Failed to build back-tracking map for MC...");
      return false;
    }

    // Get geo instance
    auto geo = larutil::Geometry::GetME();

    // Retrieve mcshower data product
    auto ev_mcs = storage->get_data<event_mcshower>("mcshower");

    // Retrieve associated cluster indices
    auto ass_cluster_v = ev_shower->association(data::kCluster,ass_keys[0]);

    for(size_t shower_index=0; shower_index < ass_cluster_v.size(); ++shower_index) {

      auto const& reco_shower = ev_shower->at(shower_index);

      auto res = fBTAlg.ShowerCorrectness(ass_cluster_v[shower_index]);

      size_t mcs_index = res.first;

      fTreeParams.match_correctness = res.second;

      if(fTreeParams.match_correctness < 0) {
	print(msg::kERROR,__FUNCTION__,
	      Form("Failed to find a corresponding MCShower for shower %zu",shower_index)
	      );
	continue;
      }

      auto const& mc_shower = ev_mcs->at(mcs_index);

      // MC Info
      fTreeParams.mc_x = mc_shower.DaughterPosition()[0];
      fTreeParams.mc_y = mc_shower.DaughterPosition()[1];
      fTreeParams.mc_z = mc_shower.DaughterPosition()[2];

      fTreeParams.mc_energy = mc_shower.DaughterMomentum().at(3);
      fTreeParams.mc_pdgid  = mc_shower.MotherPDGID();
      fTreeParams.mc_containment = mc_shower.DaughterMomentum().at(3) / mc_shower.MotherMomentum().at(3);

      fTreeParams.mc_dcosx = mc_shower.DaughterMomentum().at(0) / fTreeParams.mc_energy;
      fTreeParams.mc_dcosy = mc_shower.DaughterMomentum().at(1) / fTreeParams.mc_energy;
      fTreeParams.mc_dcosz = mc_shower.DaughterMomentum().at(2) / fTreeParams.mc_energy;

      // Reco vtx
      fTreeParams.reco_x = reco_shower.ShowerStart()[0];
      fTreeParams.reco_y = reco_shower.ShowerStart()[1];
      fTreeParams.reco_z = reco_shower.ShowerStart()[2];

      // Reco angle
      fTreeParams.reco_dcosx = reco_shower.Direction()[0];
      fTreeParams.reco_dcosy = reco_shower.Direction()[1];
      fTreeParams.reco_dcosz = reco_shower.Direction()[2];

      // Reco - MC angle diff
      fTreeParams.mc_reco_anglediff = acos( fTreeParams.reco_dcosx * fTreeParams.mc_dcosx +
					    fTreeParams.reco_dcosy * fTreeParams.mc_dcosy +
					    fTreeParams.reco_dcosz * fTreeParams.mc_dcosz ) / 3.14159265359 * 180.;
      // Reco - MC vtx distance
      fTreeParams.mc_reco_dist = sqrt( pow(fTreeParams.reco_x - fTreeParams.mc_x,2) +
				       pow(fTreeParams.reco_y - fTreeParams.mc_y,2) +
				       pow(fTreeParams.reco_z - fTreeParams.mc_z,2) );

      // Reco cluster efficiency & purity
      fTreeParams.cluster_eff = 1.;
      fTreeParams.cluster_pur = 1.;
      for(auto const& cluster_index : ass_cluster_v[shower_index]) {

	auto ep = fBTAlg.ClusterEP(cluster_index, mcs_index);

	if(ep.first==0 && ep.second==0) continue;

	fTreeParams.cluster_eff *= ep.first;
	fTreeParams.cluster_pur *= ep.second;

      }

      // Reco energy & dedx info
      fTreeParams.best_plane_id = reco_shower.best_plane();

      int best_plane_index = -1;
      
      auto ev_cluster = storage->get_data<event_cluster>(ass_keys[0]);

      for(size_t i=0; i < ass_cluster_v[shower_index].size(); ++i) {

	size_t cluster_index = ass_cluster_v[shower_index][i];

	if( ev_cluster->at(cluster_index).View() == reco_shower.best_plane() ) {
	  best_plane_index = i;
	  break;
	}
      }

      if(best_plane_index < 0) {
	throw ShowerRecoException(Form("Failed to identify the best plane for shower %zu",
				       shower_index)
				  );
      }

      fTreeParams.reco_energy = reco_shower.Energy().at(best_plane_index);
      fTreeParams.reco_dedx   = reco_shower.dEdx().at(best_plane_index);

      // Fill Tree
      fTree->Fill();

      //
      // Fill histograms
      //
      hMatchCorrectness->Fill(fTreeParams.match_correctness);

      hVtxDX->Fill(fTreeParams.reco_x - fTreeParams.mc_x);
      hVtxDY->Fill(fTreeParams.reco_y - fTreeParams.mc_y);
      hVtxDZ->Fill(fTreeParams.reco_z - fTreeParams.mc_z);
      hVtxDR->Fill(fTreeParams.mc_reco_dist);

      // Angular info
      hDCosX->Fill(fTreeParams.reco_dcosx - fTreeParams.mc_dcosx);
      hDCosY->Fill(fTreeParams.reco_dcosy - fTreeParams.mc_dcosy);
      hDCosZ->Fill(fTreeParams.reco_dcosz - fTreeParams.mc_dcosz);
      h3DAngleDiff->Fill( fTreeParams.mc_reco_anglediff );

      hEnergyCorr->Fill  ( fTreeParams.reco_energy, fTreeParams.mc_energy );

      hEnergyAssym->Fill ( (fTreeParams.reco_energy - fTreeParams.mc_energy) / 
			   (fTreeParams.reco_energy + fTreeParams.mc_energy) * 2. );
      
      hEnergyDiff->Fill  ( fTreeParams.reco_energy - fTreeParams.mc_energy );

      if(mDEDX.find(fTreeParams.mc_pdgid) == mDEDX.end()) 

	mDEDX.insert(std::make_pair(fTreeParams.mc_pdgid,
				    new TH1D(Form("hdEdx_PDG_%d",fTreeParams.mc_pdgid),
					     Form("Reco dE/dx for PDG = %d; dE/dx [MeV/cm]; Showers",fTreeParams.mc_pdgid),
					     100,0,50)
				    )
		     );

      hMatchedClusterEff->Fill(fTreeParams.cluster_eff);
      hMatchedClusterPur->Fill(fTreeParams.cluster_pur);

      mDEDX[fTreeParams.mc_pdgid]->Fill(fTreeParams.reco_dedx);

      hBestPlane->Fill(fTreeParams.best_plane_id);

      fTree->Fill();

    }
  
    return true;
  }

  bool ShowerQuality::finalize() {

    if(_fout) {

      // Write shower histograms if any entry made
      if(hMatchCorrectness->GetEntries()) {
	
	hMatchCorrectness->Write();

	hVtxDX->Write();
	hVtxDY->Write();
	hVtxDZ->Write();
	hVtxDR->Write();
	
	hDCosX->Write();
	hDCosY->Write();
	hDCosZ->Write();
	h3DAngleDiff->Write();
	
	hEnergyCorr->Write();
	hEnergyAssym->Write();
	hEnergyDiff->Write();

	for(auto& h_pair : mDEDX)
	  
	  h_pair.second->Write();

	hMatchedClusterPur->Write();
	hMatchedClusterEff->Write();

	hBestPlane->Write();

	fTree->Write();
      }

    }

    return true;
  }

  void ShowerQuality::InitializeAnaTree()
  {
    if(fTree) delete fTree;

    fTree = new TTree("fShowerQualityTree","");

    fTree->Branch("reco_x",&fTreeParams.reco_x,"reco_x/D");
    fTree->Branch("reco_y",&fTreeParams.reco_y,"reco_y/D");
    fTree->Branch("reco_z",&fTreeParams.reco_z,"reco_z/D");
    fTree->Branch("reco_dcosx",&fTreeParams.reco_dcosx,"reco_dcosx/D");
    fTree->Branch("reco_dcosy",&fTreeParams.reco_dcosy,"reco_dcosy/D");
    fTree->Branch("reco_dcosz",&fTreeParams.reco_dcosz,"reco_dcosz/D");
    fTree->Branch("reco_energy",&fTreeParams.reco_energy,"reco_energy/D");

    fTree->Branch("best_plane_id",&fTreeParams.best_plane_id,"best_plane_id/i");

    fTree->Branch("mc_x",&fTreeParams.mc_x,"mc_x/D");
    fTree->Branch("mc_y",&fTreeParams.mc_y,"mc_y/D");
    fTree->Branch("mc_z",&fTreeParams.mc_z,"mc_z/D");
    fTree->Branch("mc_dcosx",&fTreeParams.mc_dcosx,"mc_dcosx/D");
    fTree->Branch("mc_dcosy",&fTreeParams.mc_dcosy,"mc_dcosy/D");
    fTree->Branch("mc_dcosz",&fTreeParams.mc_dcosz,"mc_dcosz/D");
    fTree->Branch("mc_energy",&fTreeParams.mc_energy,"mc_energy/D");

    fTree->Branch("reco_dedx",&fTreeParams.reco_dedx,"reco_dedx/D");
    fTree->Branch("mc_pdgid",&fTreeParams.mc_pdgid,"mc_pdgid/i");

    fTree->Branch("mc_reco_anglediff",&fTreeParams.mc_reco_anglediff,"mc_reco_anglediff/D");
    fTree->Branch("mc_reco_dist",&fTreeParams.mc_reco_dist,"mc_reco_dist/D");
    
    fTree->Branch("mc_containment",&fTreeParams.mc_containment,"mc_containment/D");

    fTree->Branch("match_correctness",&fTreeParams.match_correctness,"match_correctness/D");
    fTree->Branch("cluster_eff",&fTreeParams.cluster_eff,"cluster_eff/D");
    fTree->Branch("cluster_pur",&fTreeParams.cluster_pur,"cluster_pur/D");

  }

}
#endif
