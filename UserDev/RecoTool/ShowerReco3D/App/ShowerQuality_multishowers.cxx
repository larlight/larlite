#ifndef SHOWERQUALITY_MULTISHOWERS_CXX
#define SHOWERQUALITY_MULTISHOWERS_CXX

#include "ShowerQuality_multishowers.h"
#include "LArUtil/Geometry.h"
#include "DataFormat/simch.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/trigger.h"
#include "DataFormat/mctruth.h"

namespace larlite {

ShowerQuality_multishowers::ShowerQuality_multishowers() {

  _name = "ShowerQuality_multishowers";
  _fout = 0;

  fShowerProducer  = "";

  _mcShowerQuality = true;

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

  fTree = nullptr;

  _numRecoShowers = 0;
}

bool ShowerQuality_multishowers::initialize() {

  if (fShowerProducer.empty()) {
    print(msg::kERROR, __FUNCTION__, "Shower producer's name is not set!");
    return false;
  }

  auto geo = larutil::Geometry::GetME();

  //
  // Matching correctness histogram initialization
  //
  if (hMatchCorrectness) delete hMatchCorrectness;
  hMatchCorrectness = new TH1D("hMatchCorrectness",
                               "Shower 2D Cluster Matching Correctness; Correctness; Showers",
                               101, -0.005, 1.005);

  //
  // 3D Vtx (start point) MC/Reco comparison histogram initialization
  //
  if (hVtxDX) delete hVtxDX;
  if (hVtxDY) delete hVtxDY;
  if (hVtxDZ) delete hVtxDZ;
  if (hVtxDR) delete hVtxDR;

  hVtxDX = new TH1D("hVtxDX",
                    "Reco - MC Start X [cm] Displacement; #DeltaX [cm]; Showers",
                    200, -100, 100);

  hVtxDY = new TH1D("hVtxDY",
                    "Reco - MC Start Y [cm] Displacement; #DeltaY [cm]; Showers",
                    200, -100, 100);

  hVtxDZ = new TH1D("hVtxDZ",
                    "Reco - MC Start Z [cm] Displacement; #DeltaZ [cm]; Showers",
                    200, -100, 100);

  hVtxDR = new TH1D("hVtxDR",
                    "Reco - MC Start 3D Vtx Displacement; #DeltaR [cm]; Showers",
                    200, -100, 100);

  //
  // 3D Angular MC/Reco comparison histogram initialization
  //
  if (hDCosX) delete hDCosX;
  if (hDCosY) delete hDCosY;
  if (hDCosZ) delete hDCosZ;
  if (h3DAngleDiff) delete h3DAngleDiff;

  hDCosX = new TH1D("hDCosX",
                    "Direction Unit Vector Reco - MC #DeltaX; #DeltaCosX; Showers",
                    100, -2, 2);

  hDCosY = new TH1D("hDCosY",
                    "Direction Unit Vector Reco - MC #DeltaY; #DeltaCosY; Showers",
                    100, -2, 2);

  hDCosZ = new TH1D("hDCosZ",
                    "Direction Unit Vector Reco - MC #DeltaZ; #DeltaCosZ; Showers",
                    100, -2, 2);

  h3DAngleDiff = new TH1D("h3DAngleDiff",
                          "3D Opening Angle Between Reco & MC; Opening Angle [degrees]; Showers",
                          181, -0.5, 180.5);

  //
  // Energy MC/Reco comparison histogram initialization
  //
  if (hEnergyCorr)  delete hEnergyCorr;
  if (hEnergyAssym) delete hEnergyAssym;
  if (hEnergyDiff)  delete hEnergyDiff;

  hEnergyCorr = new TH2D("hEnergyCorr",
                         "Reco (x) vs. MC (y) Energy Comparison; Reco Energy [MeV]; MC Energy [MeV]",
                         200, 0, 1000, 200, 0, 1000);

  hEnergyAssym = new TH1D("hEnergyAssym",
                          "MC - Reco Energy Fractional Difference; Assymetry; Showers",
                          201, -1.005, 1.005);

  hEnergyDiff = new TH1D("hEnergyDiff",
                         "MC - Reco Energy Difference; Energy Difference [MeV]; Showers",
                         200, 0, 1000);

  //
  // Shower cluster purity & efficiency histograms initialization
  //
  if (hMatchedClusterEff) delete hMatchedClusterEff;
  if (hMatchedClusterPur) delete hMatchedClusterPur;

  hMatchedClusterEff = new TH1D("hMatchedClusterEff_PlaneCombo",
                                "Matched Shower Cluster's Charge Efficiency; Efficiency; Clusters",
                                101, -0.005, 1.005);

  hMatchedClusterPur = new TH1D("hMatchedClusterPur_PlaneCombo",
                                "Matched Shower Cluster's Charge Purity; Purity; Clusters",
                                101, -0.005, 1.005);


  //
  // Best plane ID histogram initialization
  //
  hBestPlane = new TH1D("hBestPlane",
                        "Best Plane (for energy & dE/dx estimate); Plane ID; Showers",
                        geo->Nplanes(), -0.5, geo->Nplanes() - 0.5);

  InitializeAnaTree();

  return true;
}

bool ShowerQuality_multishowers::analyze(storage_manager* storage) {

  //auto geo = larutil::Geometry::GetME();

  // retrieve MCTruth if available
  int generation_tick = 0;
  auto ev_mctruth = storage->get_data<event_mctruth>("generator");
  if (ev_mctruth && (ev_mctruth->size() > 0) ){
    auto const& parts = ev_mctruth->at(0).GetParticles();
    for (auto const& part : parts){
      if (part.StatusCode() == 1){
	//std::cout << "Interaction time : " << part.Trajectory()[0].T()
	//	    << "\t w/ PDG " << part.PdgCode() << std::endl;
	generation_tick = (int)(part.Trajectory()[0].T() / 500.);
	break;
      }
    }
  }
  
  // retrieve trigger time
  int trigger_tick = 0;
  if (_use_trigger){
    auto trig = storage->get_data<trigger>("triggersim");
    if (!trig){
      print(msg::kERROR,__FUNCTION__,"Trigger data product not found! Required to figure out hit <-> simch offset. Use setUseTrigger(False) to not use a trigger time-offset");
      return true;
    }
    //std::cout << "Trigger time @ " << trig->TriggerTime() << std::endl;
    trigger_tick = (int)(-343.75 / 500.);
  }// if we should use the trigger time

  fBTAlg.setTickOffset( 2255 + trigger_tick + generation_tick );
  
  // Retrieve mcshower data product
  auto ev_mcs = storage->get_data<event_mcshower>("mcreco");

  if (!ev_mcs || !(ev_mcs->size())) {
    print(msg::kERROR, __FUNCTION__, "MCShower data product not found!");
    return false;
  }

  // get event-information
  _event  = ev_mcs->event_id();
  _run    = ev_mcs->run();
  _subrun = ev_mcs->subrun();

  // Retrieve simch data product
  auto ev_simch = storage->get_data<event_simch>("largeant");
  if (!ev_simch || !(ev_simch->size())) {
    print(msg::kERROR, __FUNCTION__, "SimChannel data product not found!");
    return false;
  }

  // Get shower
  auto ev_shower = storage->get_data<event_shower>(fShowerProducer);
  if (!ev_shower) {
    print(msg::kERROR, __FUNCTION__,
          Form("Did not find shower produced by \"%s\"", fShowerProducer.c_str())
         );
    return false;
  }
  if (!ev_shower->size()) return false;
  _numRecoShowers += ev_shower->size();

  // get associated clusters
  event_cluster* ev_cluster = nullptr;
  auto const& ass_cluster_v = storage->find_one_ass(ev_shower->id(), ev_cluster, ev_shower->name());

  if (!ev_cluster)
    print(msg::kERROR, __FUNCTION__, Form("No associated cluster found to a shower produced by \"%s\"", fShowerProducer.c_str()));
  else if (ev_cluster->size() < 1) {
    print(msg::kERROR, __FUNCTION__, Form("There are 0 clusters in this event! Skipping......"));
    return false;
  }

  // get associated hits
  event_hit* ev_hit = nullptr;
  auto const& ass_hit_v = storage->find_one_ass(ev_cluster->id(), ev_hit, ev_cluster->name());

  if (!ev_hit)
    print(msg::kERROR, __FUNCTION__, Form("No associated hit found to a shower produced by \"%s\"", ev_cluster->name().c_str()));
  else if (ev_hit->size() < 1) {
    print(msg::kERROR, __FUNCTION__, Form("There are 0 hits in this event! Skipping......"));
    return false;
  }


  // Create G4 track ID vector for which we are interested in
  std::vector<std::vector<unsigned int> > g4_trackid_v;
  std::vector<unsigned int> mc_index_v;
  g4_trackid_v.reserve(ev_mcs->size());
  for (size_t mc_index = 0; mc_index < ev_mcs->size(); ++mc_index) {
    auto const& mcs = (*ev_mcs)[mc_index];
    double energy = mcs.DetProfile().E();
    std::vector<unsigned int> id_v;
    id_v.reserve(mcs.DaughterTrackID().size());
    if ( _mc_energy_min < energy && energy < _mc_energy_max ) {
      for (auto const& id : mcs.DaughterTrackID()) {
        if (id == mcs.TrackID()) continue;
        id_v.push_back(id);
      }
      id_v.push_back(mcs.TrackID());
      g4_trackid_v.push_back(id_v);
      mc_index_v.push_back(mc_index);
    }
  }

  if (!fBTAlg.BuildMap(g4_trackid_v, *ev_simch, *ev_hit, ass_hit_v)) {
    print(msg::kERROR, __FUNCTION__, "Failed to build back-tracking map for MC...");
    return false;
  }

  // Find the best-representative reco-ed Shower given an MCShower
  std::vector<std::vector<double> > shower_mcq_vv(ev_shower->size(), std::vector<double>(mc_index_v.size(), 0));

  for (size_t shower_index = 0; shower_index < ass_cluster_v.size(); ++shower_index) {

    auto const& ass_cluster = ass_cluster_v[shower_index];

    std::vector< ::btutil::WireRange_t> w_v;

    for (auto const& cluster_index : ass_cluster) {

      auto const& ass_hit = ass_hit_v[cluster_index];

      w_v.reserve(ass_hit.size() + w_v.size());

      for (auto const& hit_index : ass_hit) {

        auto const& h = (*ev_hit)[hit_index];

        //w_v.push_back( ::btutil::WireRange_t( h.Channel(), h.StartTick(), h.EndTick()) );
	w_v.push_back( ::btutil::WireRange_t( h.Channel(),
					      h.PeakTime() - h.RMS() + 2255 + trigger_tick + generation_tick,
					      h.PeakTime() + h.RMS() + 2255 + trigger_tick + generation_tick) );
		       
      }
    }

    auto mcq_v = fBTAlg.BTAlg().MCQ(w_v);

    auto& shower_mcq_v = shower_mcq_vv[shower_index];

    for (size_t mcs_index = 0; mcs_index < (mcq_v.size() - 1); ++mcs_index) {

      shower_mcq_v[mcs_index] = mcq_v[mcs_index];

    }
  }

  // are we filling once per Reco shower or once per MC shower?
  mcshower mc_shower;
  shower reco_shower;
  size_t best_index;
  size_t mcs_index;
  size_t shower_index;
  // if filling once per MC shower
  if (_mcShowerQuality)
  {
    // Loop over MCShower and inspect corresponding shower quality
    for (mcs_index = 0; mcs_index < mc_index_v.size(); ++mcs_index) {

      mc_shower = (*ev_mcs)[mc_index_v[mcs_index]];

      // Search for the best representative shower
      best_index = shower_mcq_vv.size();
      double max_mcq = 0;
      for (shower_index = 0; shower_index < shower_mcq_vv.size(); ++shower_index) {

	//std::cout << "Found MC shower w/" <<  shower_mcq_vv[shower_index][mcs_index] << std::endl;
        if (shower_mcq_vv[shower_index][mcs_index] > max_mcq){
	  max_mcq = shower_mcq_vv[shower_index][mcs_index];
          best_index = shower_index;
	}

      }
      //std::cout << "Max Q : " << max_mcq << std::endl << std::endl;

      if (best_index == shower_mcq_vv.size()) {
	// fill quality info only for MC shower
	FillQualityInfo(mc_shower);
        //print(msg::kERROR, __FUNCTION__,"Failed to find a corresponding shower for MCShower");
	continue;
      }
      
      reco_shower = (*ev_shower)[best_index];

      FillQualityInfo(reco_shower, mc_shower, best_index, mcs_index, max_mcq, ass_cluster_v);
    }
  }// if filling once per MC shower
  // if filling once per RECO shower
  else {

    // Loop over RECOShower and inspect corresponding shower quality
    for (shower_index = 0; shower_index < shower_mcq_vv.size(); ++shower_index) {

      reco_shower = (*ev_shower)[shower_index];

      // search for the best representative mc shower
      best_index = mc_index_v.size();
      double max_mcq = 0;

      for (size_t mcs_index = 0; mcs_index < mc_index_v.size(); ++mcs_index) {

        if ( shower_mcq_vv[shower_index][mcs_index] > max_mcq){
	  max_mcq = shower_mcq_vv[shower_index][mcs_index];
          best_index = mcs_index;
	}
      }

      if (best_index == mc_index_v.size()) {
        continue;
      }

      mc_shower = (*ev_mcs)[mc_index_v[best_index]];

      FillQualityInfo(reco_shower, mc_shower, shower_index, best_index, max_mcq, ass_cluster_v);
    }// if filling once per RECO shower

  }

  return true;
}

bool ShowerQuality_multishowers::finalize() {

  std::cout << "Number of reco showers: " << _numRecoShowers << std::endl;

  if (_fout) {


    // Write shower histograms if any entry made
    if (hMatchCorrectness->GetEntries()) {

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

      for (auto& h_pair : mDEDX)

        h_pair.second->Write();

      hMatchedClusterPur->Write();
      hMatchedClusterEff->Write();

      hBestPlane->Write();

      fTree->Write();
    }

  }

  return true;
}

  void ShowerQuality_multishowers::FillQualityInfo(const shower& reco_shower, const mcshower& mc_shower,
						   const size_t& shower_index, const size_t& mcshower_index,
						   const double& mcq,
						   const AssSet_t& ass_cluster_v)
{

  auto res = fBTAlg.MatchCorrectness(ass_cluster_v[shower_index]);

  fTreeParams.match_correctness = res.second;

  if (fTreeParams.match_correctness < 0) {
    print(msg::kERROR, __FUNCTION__,
          Form("Failed to find a corresponding MCShower for shower %zu", shower_index)
         );
    return;
  }

  // MC Info
  fTreeParams.mc_x = mc_shower.DetProfile().X();
  fTreeParams.mc_y = mc_shower.DetProfile().Y();
  fTreeParams.mc_z = mc_shower.DetProfile().Z();
  fTreeParams.mc_t = mc_shower.DetProfile().T();

  fTreeParams.mc_q    = mc_shower.Charge(2);
  fTreeParams.mc_q_bt = mcq; // the charge identified when using the back-tracker

  fTreeParams.mc_energy = mc_shower.DetProfile().E();
  fTreeParams.mc_pdgid  = mc_shower.PdgCode();
  fTreeParams.mc_containment = mc_shower.DetProfile().E() / mc_shower.Start().E();

  //fTreeParams.mc_dcosx = mc_shower.DetProfile().Px() / fTreeParams.mc_energy;
  //fTreeParams.mc_dcosy = mc_shower.DetProfile().Py() / fTreeParams.mc_energy;
  //fTreeParams.mc_dcosz = mc_shower.DetProfile().Pz() / fTreeParams.mc_energy;
  fTreeParams.mc_dcosx = mc_shower.Start().Px() / mc_shower.Start().E();
  fTreeParams.mc_dcosy = mc_shower.Start().Py() / mc_shower.Start().E();
  fTreeParams.mc_dcosz = mc_shower.Start().Pz() / mc_shower.Start().E();

  // Reco vtx
  fTreeParams.reco_x = reco_shower.ShowerStart()[0] + 124.5;
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
  fTreeParams.mc_reco_dist = sqrt( pow(fTreeParams.reco_x - fTreeParams.mc_x, 2) +
                                   pow(fTreeParams.reco_y - fTreeParams.mc_y, 2) +
                                   pow(fTreeParams.reco_z - fTreeParams.mc_z, 2) );

  // Reco cluster efficiency & purity
  fTreeParams.cluster_eff = 1.;
  fTreeParams.cluster_pur = 1.;
  for (auto const& cluster_index : ass_cluster_v[shower_index]) {
    auto ep = fBTAlg.ClusterEP(cluster_index, mcshower_index);
    if (ep.first == 0 && ep.second == 0) continue;
    fTreeParams.cluster_eff *= ep.first;
    fTreeParams.cluster_pur *= ep.second;
  }

  // Reco energy & dedx info
  int best_plane = reco_shower.best_plane();
  fTreeParams.best_plane_id = best_plane;
  if ((best_plane < 0) or (best_plane > 2))
    best_plane = 2;

  fTreeParams.reco_energy = reco_shower.Energy_v().at(best_plane);
  fTreeParams.reco_energy_U = reco_shower.Energy_v().at(0);
  fTreeParams.reco_energy_V = reco_shower.Energy_v().at(1);
  fTreeParams.reco_energy_Y = reco_shower.Energy_v().at(2);

  fTreeParams.reco_dqdx_U   = reco_shower.dQdx_v().at(0);
  fTreeParams.reco_dqdx_V   = reco_shower.dQdx_v().at(1);
  fTreeParams.reco_dqdx_Y   = reco_shower.dQdx_v().at(2);
  
  fTreeParams.reco_dedx     = reco_shower.dEdx_v().at(best_plane);
  fTreeParams.reco_dedx_U   = reco_shower.dEdx_v().at(0);
  fTreeParams.reco_dedx_V   = reco_shower.dEdx_v().at(1);
  fTreeParams.reco_dedx_Y   = reco_shower.dEdx_v().at(2);

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

  hEnergyDiff->Fill  ( fTreeParams.mc_energy - fTreeParams.reco_energy );

  if (mDEDX.find(fTreeParams.mc_pdgid) == mDEDX.end())

    mDEDX.insert(std::make_pair(fTreeParams.mc_pdgid,
                                new TH1D(Form("hdEdx_PDG_%d", fTreeParams.mc_pdgid),
                                         Form("Reco dE/dx for PDG = %d; dE/dx [MeV/cm]; Showers", fTreeParams.mc_pdgid),
                                         100, 0, 50)
                               )
                );

  hMatchedClusterEff->Fill(fTreeParams.cluster_eff);
  hMatchedClusterPur->Fill(fTreeParams.cluster_pur);

  mDEDX[fTreeParams.mc_pdgid]->Fill(fTreeParams.reco_dedx);

  hBestPlane->Fill(fTreeParams.best_plane_id);

    // reco match?
  fTreeParams.match = 1;

  // Fill Tree
  fTree->Fill();

}


  
  void ShowerQuality_multishowers::FillQualityInfo(const mcshower& mc_shower) {
    
    // MC Info
    fTreeParams.mc_x = mc_shower.DetProfile().X();
    fTreeParams.mc_y = mc_shower.DetProfile().Y();
    fTreeParams.mc_z = mc_shower.DetProfile().Z();
    fTreeParams.mc_t = mc_shower.DetProfile().T();
    
    fTreeParams.mc_q    = mc_shower.Charge(2);
    
    fTreeParams.mc_energy = mc_shower.DetProfile().E();
    fTreeParams.mc_pdgid  = mc_shower.PdgCode();
    fTreeParams.mc_containment = mc_shower.DetProfile().E() / mc_shower.Start().E();
    
    fTreeParams.mc_dcosx = mc_shower.Start().Px() / mc_shower.Start().E();
    fTreeParams.mc_dcosy = mc_shower.Start().Py() / mc_shower.Start().E();
    fTreeParams.mc_dcosz = mc_shower.Start().Pz() / mc_shower.Start().E();
    
    
    // reco match?
    fTreeParams.match = 0;
    
    // Fill Tree
    fTree->Fill();
    
  }
  
  
  
  
  void ShowerQuality_multishowers::InitializeAnaTree()
  {
    if (fTree) delete fTree;
    
    fTree = new TTree("fShowerQualityTree", "");
    
    // event information
  fTree->Branch("event",&_event,"event/I");
  fTree->Branch("run",&_run,"run/I");
  fTree->Branch("subrun",&_subrun,"subrun/I");

  fTree->Branch("reco_x", &fTreeParams.reco_x, "reco_x/D");
  fTree->Branch("reco_y", &fTreeParams.reco_y, "reco_y/D");
  fTree->Branch("reco_z", &fTreeParams.reco_z, "reco_z/D");
  fTree->Branch("reco_dcosx", &fTreeParams.reco_dcosx, "reco_dcosx/D");
  fTree->Branch("reco_dcosy", &fTreeParams.reco_dcosy, "reco_dcosy/D");
  fTree->Branch("reco_dcosz", &fTreeParams.reco_dcosz, "reco_dcosz/D");
  fTree->Branch("reco_energy", &fTreeParams.reco_energy, "reco_energy/D");
  fTree->Branch("reco_energy_U", &fTreeParams.reco_energy_U, "reco_energy_U/D");
  fTree->Branch("reco_energy_V", &fTreeParams.reco_energy_V, "reco_energy_V/D");
  fTree->Branch("reco_energy_Y", &fTreeParams.reco_energy_Y, "reco_energy_Y/D");

  fTree->Branch("best_plane_id", &fTreeParams.best_plane_id, "best_plane_id/i");

  fTree->Branch("mc_x", &fTreeParams.mc_x, "mc_x/D");
  fTree->Branch("mc_y", &fTreeParams.mc_y, "mc_y/D");
  fTree->Branch("mc_z", &fTreeParams.mc_z, "mc_z/D");
  fTree->Branch("mc_t", &fTreeParams.mc_t, "mc_t/D");
  fTree->Branch("mc_q", &fTreeParams.mc_q, "mc_q/D");
  fTree->Branch("mc_q_bt", &fTreeParams.mc_q_bt, "mc_q_bt/D");
  fTree->Branch("mc_dcosx", &fTreeParams.mc_dcosx, "mc_dcosx/D");
  fTree->Branch("mc_dcosy", &fTreeParams.mc_dcosy, "mc_dcosy/D");
  fTree->Branch("mc_dcosz", &fTreeParams.mc_dcosz, "mc_dcosz/D");
  fTree->Branch("mc_energy", &fTreeParams.mc_energy, "mc_energy/D");

  fTree->Branch("reco_dqdx_U", &fTreeParams.reco_dqdx_U, "reco_dqdx_U/D");
  fTree->Branch("reco_dqdx_V", &fTreeParams.reco_dqdx_V, "reco_dqdx_V/D");
  fTree->Branch("reco_dqdx_Y", &fTreeParams.reco_dqdx_Y, "reco_dqdx_Y/D");
  
  fTree->Branch("reco_dedx", &fTreeParams.reco_dedx, "reco_dedx_/D");
  fTree->Branch("reco_dedx_U", &fTreeParams.reco_dedx_U, "reco_dedx_U/D");
  fTree->Branch("reco_dedx_V", &fTreeParams.reco_dedx_V, "reco_dedx_V/D");
  fTree->Branch("reco_dedx_Y", &fTreeParams.reco_dedx_Y, "reco_dedx_Y/D");
  fTree->Branch("mc_pdgid", &fTreeParams.mc_pdgid, "mc_pdgid/i");

  fTree->Branch("mc_reco_anglediff", &fTreeParams.mc_reco_anglediff, "mc_reco_anglediff/D");
  fTree->Branch("mc_reco_dist", &fTreeParams.mc_reco_dist, "mc_reco_dist/D");

  fTree->Branch("mc_containment", &fTreeParams.mc_containment, "mc_containment/D");

  fTree->Branch("match_correctness", &fTreeParams.match_correctness, "match_correctness/D");
  fTree->Branch("cluster_eff", &fTreeParams.cluster_eff, "cluster_eff/D");
  fTree->Branch("cluster_pur", &fTreeParams.cluster_pur, "cluster_pur/D");

  fTree->Branch("match",&fTreeParams.match,"match/I");

}

}
#endif
