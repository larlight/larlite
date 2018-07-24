#ifndef PIZEROMASS_CXX
#define PIZEROMASS_CXX

#include "Pi0Mass.h"
// #include "LArUtil/Geometry.h"
// #include "DataFormat/cluster.h"
// #include "DataFormat/hit.h"
namespace larlite {

Pi0Mass::Pi0Mass() {

  _name = "Pi0Mass";
  _fout = 0;

  fShowerProducer  = "";

  // fShowerTree = nullptr;
  fEventTree = nullptr;

}

bool Pi0Mass::initialize() {


  if (fShowerProducer.empty()) {
    print(msg::kERROR, __FUNCTION__, "Shower producer's name is not set!");
    return false;
  }

  InitializeAnaTrees();

  return true;
}

bool Pi0Mass::analyze(storage_manager* storage) {

  ResetEventTreeParams();

  _alg.InitializeForEvent(storage, fShowerProducer);

  // Retrieve mcshower data product
  auto ev_mcs = storage->get_data<event_mcshower>("mcreco");

  if (!ev_mcs || !(ev_mcs->size()) || ev_mcs->size() < 2) {
    print(msg::kERROR, __FUNCTION__, "MCShower data product not found!");
    return false;
  }

  _event  = storage->event_id();
  _run    = storage->run_id();
  _subrun = storage->subrun_id();

  if ( ev_mcs->size() > 2 ) {
    print(msg::kWARNING, __FUNCTION__, Form("Run %d, subrun %d, event %d has %d mc showers!", _run, _subrun, _event, ev_mcs->size() ) );
  }

  // Before getting the reconstructed showers, we store some true (mcshower) information
  // to be used as the denominator in efficiency calculations (n reco showers / n true showers, etc)
  fEventTreeParams.n_mcshowers = ev_mcs->size();
  TVector3 PerfectRecoDir1, PerfectRecoDir2, MCStartPoint1, MCStartPoint2;
  if ( ev_mcs->at(0).DetProfile().E() > ev_mcs->at(1).DetProfile().E() ) { 
    fEventTreeParams.PerfectRecoE1 = ev_mcs->at(0).DetProfile().E();
    fEventTreeParams.PerfectRecoE2 = ev_mcs->at(1).DetProfile().E();
    PerfectRecoDir1.SetXYZ( ev_mcs->at(0).Start().Px() / ev_mcs->at(0).Start().E(),
                            ev_mcs->at(0).Start().Py() / ev_mcs->at(0).Start().E(),
                            ev_mcs->at(0).Start().Pz() / ev_mcs->at(0).Start().E() );
    PerfectRecoDir2.SetXYZ( ev_mcs->at(1).Start().Px() / ev_mcs->at(1).Start().E(),
                            ev_mcs->at(1).Start().Py() / ev_mcs->at(1).Start().E(),
                            ev_mcs->at(1).Start().Pz() / ev_mcs->at(1).Start().E() );
    fEventTreeParams.mcs_E1 = ev_mcs->at(0).Start().E();
    fEventTreeParams.mcs_E2 = ev_mcs->at(1).Start().E();
    fEventTreeParams.mc_containment1 = ev_mcs->at(0).DetProfile().E() / ev_mcs->at(0).Start().E();
    fEventTreeParams.mc_containment2 = ev_mcs->at(1).DetProfile().E() / ev_mcs->at(1).Start().E();
    MCStartPoint1.SetXYZ( ev_mcs->at(0).Start().X(), ev_mcs->at(0).Start().Y(),
                          ev_mcs->at(0).Start().Z() );
    MCStartPoint2.SetXYZ( ev_mcs->at(1).Start().X(), ev_mcs->at(1).Start().Y(),
                          ev_mcs->at(1).Start().Z() );

  } else {
    fEventTreeParams.PerfectRecoE1 = ev_mcs->at(1).DetProfile().E();
    fEventTreeParams.PerfectRecoE2 = ev_mcs->at(0).DetProfile().E();
    PerfectRecoDir1.SetXYZ( ev_mcs->at(1).Start().Px() / ev_mcs->at(1).Start().E(),
                            ev_mcs->at(1).Start().Py() / ev_mcs->at(1).Start().E(),
                            ev_mcs->at(1).Start().Pz() / ev_mcs->at(1).Start().E() );
    PerfectRecoDir2.SetXYZ( ev_mcs->at(0).Start().Px() / ev_mcs->at(0).Start().E(),
                            ev_mcs->at(0).Start().Py() / ev_mcs->at(0).Start().E(),
                            ev_mcs->at(0).Start().Pz() / ev_mcs->at(0).Start().E() );
    fEventTreeParams.mcs_E1 = ev_mcs->at(1).Start().E();
    fEventTreeParams.mcs_E2 = ev_mcs->at(0).Start().E();
    fEventTreeParams.mc_containment1 = ev_mcs->at(1).DetProfile().E() / ev_mcs->at(1).Start().E();
    fEventTreeParams.mc_containment2 = ev_mcs->at(0).DetProfile().E() / ev_mcs->at(0).Start().E();
    MCStartPoint1.SetXYZ( ev_mcs->at(1).Start().X(), ev_mcs->at(1).Start().Y(),
                          ev_mcs->at(1).Start().Z() );
    MCStartPoint2.SetXYZ( ev_mcs->at(0).Start().X(), ev_mcs->at(0).Start().Y(),
                          ev_mcs->at(0).Start().Z() );
  }

  fEventTreeParams.PerfectRecoTheta = PerfectRecoDir1.Angle( PerfectRecoDir2 );
  fEventTreeParams.PerfectRecoCosTheta = cos( fEventTreeParams.PerfectRecoTheta );
  fEventTreeParams.PerfectRecoPi0Mass = sqrt( 2.*fEventTreeParams.PerfectRecoE1*fEventTreeParams.PerfectRecoE2* ( 1. - fEventTreeParams.PerfectRecoCosTheta ) );
  TVector3 MCStartDist = MCStartPoint1 - MCStartPoint2;
  fEventTreeParams.mc_start_dist = MCStartDist.Mag();

  // Retrieve shower data product
  auto ev_shower = storage->get_data<event_shower>(fShowerProducer);
  if (!ev_shower) {
    print(msg::kERROR, __FUNCTION__, Form("Did not find shower produced by \"%s\"", fShowerProducer.c_str()));
    return false;
  }
  //Fill event TTree number of reconstructed showers
  fEventTreeParams.n_recoshowers = ev_shower->size();

  if (!ev_shower->size()) {
    //Fill the once-per-event TTree before you quit out early
    // fEventTree->Fill();
    return false;
  }

  if ( ev_shower->size() < 2 ) return false;

  // // Get associated clusters
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

  double MaxRecoShowerChargePerPlane[2] = { 0., 0. };
  size_t iMaxRecoShowerChargePerPlane[2] = { 0, 0 };
  size_t nClustersMaxRecoShowerChargePerPlane[2] = { 0, 0 };
  for (size_t shower_index = 0; shower_index < ev_shower->size(); ++shower_index) {
    // auto const& myrecoshower = ev_shower->at(shower_index);
    auto nclusters = ass_cluster_v[shower_index].size();
    if ( nclusters > 3 ) nclusters = 3;
    double ChargeSumPerPlane = 0;
    for ( auto icluster : ass_cluster_v[shower_index] ) {
      for ( auto ihit : ass_hit_v[icluster] )
         ChargeSumPerPlane += ev_hit->at(ihit).SummedADC();
    }
    ChargeSumPerPlane /= (double)nclusters;
    // std::cout << "ChargeSumPerPlane (shower " << shower_index << "): " << ChargeSumPerPlane << std::endl;

    if ( ChargeSumPerPlane > MaxRecoShowerChargePerPlane[0] ) { 
      MaxRecoShowerChargePerPlane[1] = MaxRecoShowerChargePerPlane[0];
      MaxRecoShowerChargePerPlane[0] = ChargeSumPerPlane;
      iMaxRecoShowerChargePerPlane[1] = iMaxRecoShowerChargePerPlane[0];
      iMaxRecoShowerChargePerPlane[0] = shower_index;
      nClustersMaxRecoShowerChargePerPlane[0] = nclusters;
    }
    else if ( ChargeSumPerPlane > MaxRecoShowerChargePerPlane[1] ) {
      MaxRecoShowerChargePerPlane[1] = ChargeSumPerPlane;
      iMaxRecoShowerChargePerPlane[1] = shower_index;
      nClustersMaxRecoShowerChargePerPlane[1] = nclusters;
    }
  }

  auto const& shower1 = ev_shower->at(iMaxRecoShowerChargePerPlane[0]);
  auto const& shower2 = ev_shower->at(iMaxRecoShowerChargePerPlane[1]);
  double E1 = 0., E2 = 0., CosTheta = 0.;
  /*
  for ( size_t i = 0; i < 3; i ++ ) {
    if ( shower1.Energy_v().at(i) > 0. ) E1 += shower1.Energy_v().at(i);
    if ( shower2.Energy_v().at(i) > 0. ) E2 += shower2.Energy_v().at(i);
  }
  E1 /= (double)nClustersMaxRecoShowerChargePerPlane[0];
  E2 /= (double)nClustersMaxRecoShowerChargePerPlane[1];
  */

  if ( shower1.Energy_v().at(2) > 0.1 ) E1 = shower1.Energy_v().at(2);
  else if ( shower1.Energy_v().at(0) > 0.1 ) E1 = shower1.Energy_v().at(0);
  else E1 = shower1.Energy_v().at(1);
  if ( shower2.Energy_v().at(2) > 0.1 ) E2 = shower2.Energy_v().at(2);
  else if ( shower2.Energy_v().at(0) > 0.1 ) E2 = shower2.Energy_v().at(0);
  else E2 = shower2.Energy_v().at(1);
  if ( E1 == 0. || E2 == 0. ) return false;
  if ( E1 > 100000. || E2 > 100000. ) return false;

  // std::cout << "E1: " << E1 << ", E2: " << E2 << std::endl;

  fEventTreeParams.RecoTheta = shower1.Direction().Angle( shower2.Direction() );
  CosTheta = cos( fEventTreeParams.RecoTheta );


  fEventTreeParams.RecoE1 = E1;
  fEventTreeParams.RecoE2 = E2;
  fEventTreeParams.RecoCosTheta = CosTheta;
  fEventTreeParams.RecoPi0Mass = sqrt( 2.*E1*E2* ( 1. - CosTheta) );

  fEventTreeParams.PerfectRecoERecoThetaPi0Mass = sqrt( 2.*fEventTreeParams.PerfectRecoE1*fEventTreeParams.PerfectRecoE2*( 1. - CosTheta ) );

  fEventTreeParams.RecoEPerfectRecoThetaPi0Mass = sqrt( 2.*E1*E2* fEventTreeParams.PerfectRecoCosTheta );

  // The reconstructed showers which match the direction of MCshowers
  fEventTreeParams.mc_reco_anglediff1 = shower1.Direction().Angle( PerfectRecoDir1 );
  fEventTreeParams.mc_reco_anglediff2 = shower2.Direction().Angle( PerfectRecoDir2 );

  TVector3 MCRecoStartDiff1 = shower1.ShowerStart() - MCStartPoint1;
  TVector3 MCRecoStartDiff2 = shower2.ShowerStart() - MCStartPoint2;

  fEventTreeParams.mc_reco_start_dist1 = MCRecoStartDiff1.Mag();
  fEventTreeParams.mc_reco_start_dist2 = MCRecoStartDiff2.Mag();

  double t1 = -1.*MCRecoStartDiff1*shower1.Direction() / shower1.Direction().Mag2();
  TVector3 dca1 = shower1.ShowerStart() + t1*shower1.Direction() - MCStartPoint1;

  double t2 = -1.*MCRecoStartDiff2*shower2.Direction() / shower2.Direction().Mag2();
  TVector3 dca2 = shower2.ShowerStart() + t2*shower2.Direction() - MCStartPoint2;

  fEventTreeParams.mc_reco_dca1 = dca1.Mag();
  fEventTreeParams.mc_reco_dca2 = dca2.Mag();

//Fill the once-per-event TTree
  fEventTree->Fill();


  return true;

}

bool Pi0Mass::finalize() {

  if (_fout) {
    // if (fShowerTree) fShowerTree->Write();
    if (fEventTree) fEventTree->Write();
  }
  return true;
}

/*
void Pi0Mass::FillQualityInfo(const shower& reco_shower, const mcshower& mc_shower, size_t shower_index)// const std::vector< const larlite::cluster * > associated_clusters)
{

  ResetShowerTreeParams();

  // MC Info
  fShowerTreeParams.mc_x = mc_shower.DetProfile().X();
  fShowerTreeParams.mc_y = mc_shower.DetProfile().Y();
  fShowerTreeParams.mc_z = mc_shower.DetProfile().Z();

  fShowerTreeParams.mc_energy = mc_shower.DetProfile().E();
  fShowerTreeParams.mc_containment = mc_shower.DetProfile().E() / mc_shower.Start().E();

  fShowerTreeParams.mc_dcosx = mc_shower.Start().Px() / mc_shower.Start().E();
  fShowerTreeParams.mc_dcosy = mc_shower.Start().Py() / mc_shower.Start().E();
  fShowerTreeParams.mc_dcosz = mc_shower.Start().Pz() / mc_shower.Start().E();
    
  // Reco vtx
  fShowerTreeParams.reco_x = reco_shower.ShowerStart()[0];
  fShowerTreeParams.reco_y = reco_shower.ShowerStart()[1];
  fShowerTreeParams.reco_z = reco_shower.ShowerStart()[2];

  // Reco angle
  fShowerTreeParams.reco_dcosx = reco_shower.Direction()[0];
  fShowerTreeParams.reco_dcosy = reco_shower.Direction()[1];
  fShowerTreeParams.reco_dcosz = reco_shower.Direction()[2];

  // Reco - MC angle diff
  fShowerTreeParams.mc_reco_anglediff = acos( fShowerTreeParams.reco_dcosx * fShowerTreeParams.mc_dcosx +
                                        fShowerTreeParams.reco_dcosy * fShowerTreeParams.mc_dcosy +
                                        fShowerTreeParams.reco_dcosz * fShowerTreeParams.mc_dcosz ) / 3.14159265359 * 180.;
  // Reco - MC vtx distance
  fShowerTreeParams.mc_reco_dist = sqrt( pow(fShowerTreeParams.reco_x - fShowerTreeParams.mc_x, 2) +
                                         pow(fShowerTreeParams.reco_y - fShowerTreeParams.mc_y, 2) +
                                         pow(fShowerTreeParams.reco_z - fShowerTreeParams.mc_z, 2) );

  // Reco cluster efficiency & purity
  auto eff_purs = _alg.ClusterEP(shower_index);
  fShowerTreeParams.cluster_eff_U = eff_purs.at(0).first;
  fShowerTreeParams.cluster_eff_V = eff_purs.at(1).first;
  fShowerTreeParams.cluster_eff_Y = eff_purs.at(2).first;
  fShowerTreeParams.cluster_pur_U = eff_purs.at(0).second;
  fShowerTreeParams.cluster_pur_V = eff_purs.at(1).second;
  fShowerTreeParams.cluster_pur_Y = eff_purs.at(2).second;

  // for (auto const& cluster_index : ass_cluster_v[shower_index]) {
  //   auto ep = fBTAlg.ClusterEP(cluster_index, mcshower_index);
  //   if (ep.first == 0 && ep.second == 0) continue;
  //   fShowerTreeParams.cluster_eff *= ep.first;
  //   fShowerTreeParams.cluster_pur *= ep.second;
  // }

  fShowerTreeParams.reco_energy_U = reco_shower.Energy_v().at(0);
  fShowerTreeParams.reco_energy_V = reco_shower.Energy_v().at(1);
  fShowerTreeParams.reco_energy_Y = reco_shower.Energy_v().at(2);
  
  fShowerTreeParams.reco_dedx_U   = reco_shower.dEdx_v().at(0);
  fShowerTreeParams.reco_dedx_V   = reco_shower.dEdx_v().at(1);
  fShowerTreeParams.reco_dedx_Y   = reco_shower.dEdx_v().at(2);

  fShowerTreeParams.reco_dqdx_U = reco_shower.dQdx_v().at(0);
  fShowerTreeParams.reco_dqdx_V = reco_shower.dQdx_v().at(1);                                                           
  fShowerTreeParams.reco_dqdx_Y = reco_shower.dQdx_v().at(2);

  fShowerTreeParams.reco_length = reco_shower.Length();


  // Fill Tree
  fShowerTree->Fill();

}
*/
void Pi0Mass::InitializeAnaTrees()
{

  //////////////////////////////////////////////////////
  // This tree is filled once per reconstructed shower
  //////////////////////////////////////////////////////
/*
  if (fShowerTree) delete fShowerTree;
  fShowerTree = new TTree("fShowerTree_singleshowers", "");

  fShowerTree->Branch("event", &_event, "event/I");
  fShowerTree->Branch("run", &_run, "run/I");
  fShowerTree->Branch("subrun", &_subrun, "subrun/I");
  fShowerTree->Branch("reco_x", &fShowerTreeParams.reco_x, "reco_x/D");
  fShowerTree->Branch("reco_y", &fShowerTreeParams.reco_y, "reco_y/D");
  fShowerTree->Branch("reco_z", &fShowerTreeParams.reco_z, "reco_z/D");
  fShowerTree->Branch("reco_dcosx", &fShowerTreeParams.reco_dcosx, "reco_dcosx/D");
  fShowerTree->Branch("reco_dcosy", &fShowerTreeParams.reco_dcosy, "reco_dcosy/D");
  fShowerTree->Branch("reco_dcosz", &fShowerTreeParams.reco_dcosz, "reco_dcosz/D");
  fShowerTree->Branch("reco_energy_U", &fShowerTreeParams.reco_energy_U, "reco_energy_U/D");
  fShowerTree->Branch("reco_energy_V", &fShowerTreeParams.reco_energy_V, "reco_energy_V/D");
  fShowerTree->Branch("reco_energy_Y", &fShowerTreeParams.reco_energy_Y, "reco_energy_Y/D");
  fShowerTree->Branch("mc_x", &fShowerTreeParams.mc_x, "mc_x/D");
  fShowerTree->Branch("mc_y", &fShowerTreeParams.mc_y, "mc_y/D");
  fShowerTree->Branch("mc_z", &fShowerTreeParams.mc_z, "mc_z/D");
  fShowerTree->Branch("mc_dcosx", &fShowerTreeParams.mc_dcosx, "mc_dcosx/D");
  fShowerTree->Branch("mc_dcosy", &fShowerTreeParams.mc_dcosy, "mc_dcosy/D");
  fShowerTree->Branch("mc_dcosz", &fShowerTreeParams.mc_dcosz, "mc_dcosz/D");
  fShowerTree->Branch("reco_dqdx_U", &fShowerTreeParams.reco_dqdx_U, "reco_dqdx_U/D");
  fShowerTree->Branch("reco_dqdx_V", &fShowerTreeParams.reco_dqdx_V, "reco_dqdx_V/D");
  fShowerTree->Branch("reco_dqdx_Y", &fShowerTreeParams.reco_dqdx_Y, "reco_dqdx_Y/D");
  fShowerTree->Branch("mc_energy", &fShowerTreeParams.mc_energy, "mc_energy/D");
  fShowerTree->Branch("reco_dedx_U", &fShowerTreeParams.reco_dedx_U, "reco_dedx_U/D");
  fShowerTree->Branch("reco_dedx_V", &fShowerTreeParams.reco_dedx_V, "reco_dedx_V/D");
  fShowerTree->Branch("reco_dedx_Y", &fShowerTreeParams.reco_dedx_Y, "reco_dedx_Y/D");
  fShowerTree->Branch("mc_reco_anglediff", &fShowerTreeParams.mc_reco_anglediff, "mc_reco_anglediff/D");
  fShowerTree->Branch("mc_reco_dist", &fShowerTreeParams.mc_reco_dist, "mc_reco_dist/D");
  fShowerTree->Branch("cluster_eff_U", &fShowerTreeParams.cluster_eff_U, "cluster_eff_U/D");
  fShowerTree->Branch("cluster_eff_V", &fShowerTreeParams.cluster_eff_V, "cluster_eff_V/D");
  fShowerTree->Branch("cluster_eff_Y", &fShowerTreeParams.cluster_eff_Y, "cluster_eff_Y/D");
  fShowerTree->Branch("cluster_pur_U", &fShowerTreeParams.cluster_pur_U, "cluster_pur_U/D");
  fShowerTree->Branch("cluster_pur_V", &fShowerTreeParams.cluster_pur_V, "cluster_pur_V/D");
  fShowerTree->Branch("cluster_pur_Y", &fShowerTreeParams.cluster_pur_Y, "cluster_pur_Y/D");
  fShowerTree->Branch("mc_containment", &fShowerTreeParams.mc_containment, "mc_containment/D");
  fShowerTree->Branch("reco_length",&fShowerTreeParams.reco_length,"reco_length/D");
*/
  //////////////////////////////////////////////////////
  // This tree is filled once per event
  //////////////////////////////////////////////////////
  if (fEventTree) delete fEventTree;
  fEventTree = new TTree("fEventTree", "");

  fEventTree->Branch("n_recoshowers", &fEventTreeParams.n_recoshowers, "n_recoshowers/I");
  fEventTree->Branch("n_mcshowers", &fEventTreeParams.n_mcshowers, "n_mcshowers/I");
  fEventTree->Branch("mcs_E1", &fEventTreeParams.mcs_E1, "mcs_E1/D");
  fEventTree->Branch("mcs_E2", &fEventTreeParams.mcs_E2, "mcs_E2/D");
  fEventTree->Branch("mc_containment1", &fEventTreeParams.mc_containment1, "mc_containment1/D");
  fEventTree->Branch("mc_containment2", &fEventTreeParams.mc_containment2, "mc_containment2/D");
  fEventTree->Branch("PerfectRecoERecoThetaPi0Mass", &fEventTreeParams.PerfectRecoERecoThetaPi0Mass, "PerfectRecoERecoThetaPi0Mass/D");
  fEventTree->Branch("RecoEPerfectRecoThetaPi0Mass", &fEventTreeParams.RecoEPerfectRecoThetaPi0Mass, "RecoEPerfectRecoThetaPi0Mass/D");
  fEventTree->Branch("PerfectRecoPi0Mass", &fEventTreeParams.PerfectRecoPi0Mass, "PerfectRecoPi0Mass/D");
  fEventTree->Branch("PerfectRecoE1", &fEventTreeParams.PerfectRecoE1, "PerfectRecoE1/D");
  fEventTree->Branch("PerfectRecoE2", &fEventTreeParams.PerfectRecoE2, "PerfectRecoE2/D");
  fEventTree->Branch("PerfectRecoCosTheta", &fEventTreeParams.PerfectRecoCosTheta, "PerfectRecoCosTheta/D");
  fEventTree->Branch("PerfectRecoTheta", &fEventTreeParams.PerfectRecoTheta, "PerfectRecoTheta/D");
  fEventTree->Branch("RecoPi0Mass", &fEventTreeParams.RecoPi0Mass, "RecoPi0Mass/D");
  fEventTree->Branch("RecoE1", &fEventTreeParams.RecoE1, "RecoE1/D");
  fEventTree->Branch("RecoE2", &fEventTreeParams.RecoE2, "RecoE2/D");
  fEventTree->Branch("RecoCosTheta", &fEventTreeParams.RecoCosTheta, "RecoCosTheta/D");
  fEventTree->Branch("RecoTheta", &fEventTreeParams.RecoTheta, "RecoTheta/D");
  fEventTree->Branch("mc_reco_anglediff1", &fEventTreeParams.mc_reco_anglediff1, "mc_reco_anglediff1/D");
  fEventTree->Branch("mc_reco_anglediff2", &fEventTreeParams.mc_reco_anglediff2, "mc_reco_anglediff2/D");
  fEventTree->Branch("mc_start_dist", &fEventTreeParams.mc_start_dist, "mc_start_dist/D");
  fEventTree->Branch("mc_reco_start_dist1", &fEventTreeParams.mc_reco_start_dist1, "mc_reco_start_dist1/D");
  fEventTree->Branch("mc_reco_start_dist2", &fEventTreeParams.mc_reco_start_dist2, "mc_reco_start_dist2/D");
  fEventTree->Branch("mc_reco_dca1", &fEventTreeParams.mc_reco_dca1, "mc_reco_dca1/D");
  fEventTree->Branch("mc_reco_dca2", &fEventTreeParams.mc_reco_dca2, "mc_reco_dca2/D");
}
/*
void Pi0Mass::ResetShowerTreeParams() {

  fShowerTreeParams.reco_x = -1.; fShowerTreeParams.reco_y = -1.; fShowerTreeParams.reco_z = -1.;
  fShowerTreeParams.reco_dcosx = -1.; fShowerTreeParams.reco_dcosy = -1.; fShowerTreeParams.reco_dcosz = -1.;
  fShowerTreeParams.reco_energy_U = -1.;
  fShowerTreeParams.reco_energy_V = -1.;
  fShowerTreeParams.reco_energy_Y = -1.;
  fShowerTreeParams.reco_dedx_U = -1.;
  fShowerTreeParams.reco_dedx_V = -1.;
  fShowerTreeParams.reco_dedx_Y = -1.;
  fShowerTreeParams.reco_dqdx_U =-1.;
  fShowerTreeParams.reco_dqdx_V =-1.;
  fShowerTreeParams.reco_dqdx_Y =-1.;
  fShowerTreeParams.mc_x = -1.; fShowerTreeParams.mc_y = -1.; fShowerTreeParams.mc_z = -1.;
  fShowerTreeParams.mc_dcosx = -1.; fShowerTreeParams.mc_dcosy = -1.; fShowerTreeParams.mc_dcosz = -1.;
  fShowerTreeParams.mc_energy = -1.;
  fShowerTreeParams.mc_reco_anglediff = -1.;
  fShowerTreeParams.mc_reco_dist = -1.;
  fShowerTreeParams.cluster_eff_U = -1.234;
  fShowerTreeParams.cluster_eff_V = -1.234;
  fShowerTreeParams.cluster_eff_Y = -1.234;
  fShowerTreeParams.cluster_pur_U = -1.234;
  fShowerTreeParams.cluster_pur_V = -1.234;
  fShowerTreeParams.cluster_pur_Y = -1.234;
  fShowerTreeParams.mc_containment = -1.;
  fShowerTreeParams.reco_length = -1.;
}
*/
void Pi0Mass::ResetEventTreeParams() {
  fEventTreeParams.n_mcshowers = 0;
  fEventTreeParams.n_recoshowers = 0;
  fEventTreeParams.mcs_E1 = -1.;
  fEventTreeParams.mcs_E2 = -1.;
  fEventTreeParams.mc_containment1 = -1.;
  fEventTreeParams.mc_containment2 = -1.;
  fEventTreeParams.PerfectRecoERecoThetaPi0Mass = 0.;
  fEventTreeParams.RecoEPerfectRecoThetaPi0Mass = 0.;
  fEventTreeParams.PerfectRecoPi0Mass = 0.;
  fEventTreeParams.PerfectRecoE1 = 0.;
  fEventTreeParams.PerfectRecoE2 = 0.;
  fEventTreeParams.PerfectRecoCosTheta = -2.;
  fEventTreeParams.PerfectRecoTheta = -1000.;
  fEventTreeParams.RecoPi0Mass = 0.;
  fEventTreeParams.RecoE1 = 0.;
  fEventTreeParams.RecoE2 = 0.;
  fEventTreeParams.RecoCosTheta = -2.;
  fEventTreeParams.RecoTheta = -1000.;
  
  fEventTreeParams.mc_reco_anglediff1 = -1.;
  fEventTreeParams.mc_reco_anglediff2 = -1.;
  fEventTreeParams.mc_start_dist = -1.;
  fEventTreeParams.mc_reco_start_dist1 = -1.;
  fEventTreeParams.mc_reco_start_dist2 = -1.;
  fEventTreeParams.mc_reco_dca1 = -1.;
  fEventTreeParams.mc_reco_dca2 = -1.;
}


}
#endif
