#ifndef RECOTOOL_COMPUTEPI0MASS_CXX
#define RECOTOOL_COMPUTEPI0MASS_CXX

#include "ComputePi0Mass.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/simch.h"
#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
namespace larlite {
  
  ComputePi0Mass* ComputePi0Mass::me = 0;

  bool ComputePi0Mass::initialize() {

    if(_shower_producer.empty()) {

      print(msg::kERROR,__FUNCTION__,
	    "Shower producer name is empty!");
      return false;
    }
      

    hPi0MassPeak = new TH1D("hPi0MassPeak","Pi0 Mass Peak in MeV",100,0,300);

    hDaughterVtxDist = new TH1D("hDaughterVtxDist",
				"Pi0 Daughter Start Point Distance (Reco vs. MC); Distance [cm]; Showers",
				100,0,10);

    hDaughterAngleDiff = new TH1D("hDaughterAngleDiff",
				  "Pi0 Daughter 3D Angle Diff (Reco vs. MC); Opening Angle [degrees]; Showers",
				  100,0,180);
    return true;
  }
  
  bool ComputePi0Mass::analyze(storage_manager* storage) {

    _mass = -99999.;

    // Retrieve mcshower data product
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    
    if(!ev_mcs || !(ev_mcs->size())) {
      print(msg::kERROR,__FUNCTION__,"MCShower data product not found!");
      return false;
    }

    // Retrieve simch data product
    auto ev_simch = storage->get_data<event_simch>("largeant");
    if(!ev_simch || !(ev_simch->size())) {
      print(msg::kERROR,__FUNCTION__,"SimChannel data product not found!");
      return false;
    }

    // Load the Showers... need to run shower reconstruction first!
    auto ev_shower  = storage->get_data<event_shower>(_shower_producer);

    if(!ev_shower ) {
      print(msg::kERROR,__FUNCTION__,
	    Form("Shower by \"%s\" not found...",_shower_producer.c_str())
	    );
      return false;
    }else if(ev_shower->size()<1) {
      if(get_verbosity() <= msg::kINFO)
	print(msg::kINFO,__FUNCTION__,Form("There are 0 showers in this event! Skipping......"));      
      return false;
    }

    //skip event if !2 showers found.
    if(ev_shower->size() != 2) return false;

    // get associated clusters
    event_cluster* ev_cluster = nullptr;
    auto const& cluster_ass = storage->find_one_ass(ev_shower->id(),ev_cluster,ev_shower->name());

    if (!ev_cluster)
      throw ::showerreco::ShowerRecoException(Form("Shower product by \"%s\" does not contain association to clusters!",_shower_producer.c_str()));
    else if(ev_cluster->size()<1) {
      print(msg::kERROR,__FUNCTION__,Form("There are 0 clusters in this event! Skipping......"));      
      return false;
    }

    // get associated hits
    event_hit* ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_cluster->id(),ev_hit,ev_cluster->name());

    if (!ev_hit)
      throw ::showerreco::ShowerRecoException(Form("Shower product by \"%s\" does not contain association to hits!",_shower_producer.c_str()));
    else if(ev_hit->size()<1) {
      print(msg::kERROR,__FUNCTION__,Form("There are 0 hits in this event! Skipping......"));      
      return false;
    }

    /*
    auto cluster_producers = ev_shower->association_keys(data::kCluster);
    if(!cluster_producers.size())
     // throw fSRException.ShowerRecoException(Form("Shower product by \"%s\" does not contain association to clusters!",_shower_producer.c_str()));
      throw ::showerreco::ShowerRecoException(Form("Shower product by \"%s\" does not contain association to clusters!",_shower_producer.c_str()));

    auto ev_cluster = storage->get_data<event_cluster>(cluster_producers[0]);

    if(!ev_cluster ) {
      print(msg::kERROR,__FUNCTION__,
	    Form("Associated cluster by \"%s\" not found...",
		 cluster_producers[0].c_str())
	    );
      return false;
    }else if(ev_cluster->size()<1) {
      print(msg::kERROR,__FUNCTION__,Form("There are 0 clusters in this event! Skipping......"));      
      return false;
    }

    auto cluster_ass = ev_shower->association(ev_cluster->id());

    // Get hits
    auto cluster_hit_ass_keys = ev_cluster->association_keys(data::kHit);
    if(!(cluster_hit_ass_keys.size())) {
      print(msg::kERROR,__FUNCTION__,
	    Form("No cluster=>hit association found for \"%s\"!",ev_cluster->name().c_str())
	    );
      return false;
    }

    auto ev_hit = storage->get_data<event_hit>(cluster_hit_ass_keys[0]);
    if(!ev_hit || !(ev_hit->size())) {
      print(msg::kERROR,__FUNCTION__,"Could not retrieve a reconstructed hit!");
      return false;
    }

    // Retrieve cluster=>hit association
    auto ass_hit_v = ev_cluster->association(ev_hit->id());
    */

    // Create G4 track ID vector for which we are interested in
    std::vector<unsigned int> g4_trackid_v;
    std::vector<unsigned int> mc_index_v;
    g4_trackid_v.reserve(ev_mcs->size());
    for(size_t mc_index=0; mc_index<ev_mcs->size(); ++mc_index) {
      auto const& mcs = (*ev_mcs)[mc_index];
      g4_trackid_v.push_back(mcs.TrackID());
      mc_index_v.push_back(mc_index);
    }

    if(!fBTAlg.BuildMap(g4_trackid_v, *ev_simch, *ev_hit, ass_hit_v)) {
      print(msg::kERROR,__FUNCTION__,"Failed to build back-tracking map for MC...");
      return false;
    }

    size_t best_cluster1 = 0;
    for(auto const& index : cluster_ass[0]){
      if(ev_shower->at(0).best_plane() == ev_cluster->at(index).View()) break;
      ++best_cluster1;
    }

    size_t best_cluster2 = 0;
    for(auto const& index : cluster_ass[1]){
      if(ev_shower->at(1).best_plane() == ev_cluster->at(index).View()) break;
      ++best_cluster2;
    }

    _mass = Pi0MassFormula3D( fSECaloAlg.ShowerEnergy(ev_shower->at(0).Energy().at(best_cluster1)).first,
			      fSECaloAlg.ShowerEnergy(ev_shower->at(1).Energy().at(best_cluster2)).first,
    			      ev_shower->at(0).Direction(),
    			      ev_shower->at(1).Direction() );
    
    hPi0MassPeak->Fill(_mass);

    for(size_t shower_index=0; shower_index < ev_shower->size(); ++shower_index) {

      auto const& s = ev_shower->at(shower_index);

      std::pair<size_t,double> res = fBTAlg.ShowerCorrectness(cluster_ass[shower_index]);
      size_t mcs_index = res.first;
      double correctness = res.second;

      if(correctness) {

	auto const& mcs = ev_mcs->at(mcs_index);
	auto reco_dir = s.Direction();
	auto reco_vtx = s.ShowerStart();

	auto mc_dir = mcs.DetProfile().Momentum();
	auto mc_vtx = mcs.DetProfile().Position();

	double mag = sqrt(pow(mc_dir[0],2)+pow(mc_dir[1],2)+pow(mc_dir[2],2));
	mc_dir[0] /= mag;
	mc_dir[1] /= mag;
	mc_dir[2] /= mag;

	hDaughterVtxDist->Fill( sqrt( pow(mc_vtx[0] - reco_vtx[0],2) +
				      pow(mc_vtx[1] - reco_vtx[1],2) +
				      pow(mc_vtx[2] - reco_vtx[2],2) 
				      )
				);

	double dot_prod = 0;
	dot_prod += reco_dir[0] * mc_dir[0];
	dot_prod += reco_dir[1] * mc_dir[1];
	dot_prod += reco_dir[2] * mc_dir[2];
	
	hDaughterAngleDiff->Fill( acos(dot_prod) / TMath::Pi() * 180 );

      }

    }
    
    return true;
  }

  bool ComputePi0Mass::finalize() {

    if(_fout) { 
      _fout->cd(); 
      hPi0MassPeak->Write();
      hDaughterVtxDist->Write();
      hDaughterAngleDiff->Write();
    }
    else 
      print(msg::kERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    
  
    return true;
  }


  //Get PI0 Mass from photon directions and energy
  float ComputePi0Mass::Pi0MassFormula3D( float Energy1, float Energy2, TVector3 Direction3D_1, TVector3 Direction3D_2){
    
    float angle_3D = acos( Direction3D_1 * Direction3D_2 );

    //using a formula from 
    //http://www.hep.princeton.edu/~mcdonald/examples/piondecay.pdf      
    return pow(4. * Energy1 * Energy2 * pow(sin( 0.5 * angle_3D ),2) , 0.5);

  }

}
#endif
