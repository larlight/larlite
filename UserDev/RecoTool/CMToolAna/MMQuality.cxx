#ifndef RECOTOOL_MMQUALITY_CXX
#define RECOTOOL_MMQUALITY_CXX

#include "MMQuality.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/simch.h"
#include "DataFormat/shower.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {
  MMQuality::MMQuality() {
    
    _name="MMQuality"; 
    _fout=0;

    _mc_energy_min = 1;
    _mc_energy_max = 1000000;
    
    fClusterProducer = "";
    fShowerProducer  = "";
    
    //vShowerClusterEff.clear();
    //vShowerClusterPur.clear();
    //vShowerClusterEffVsPur.clear();
    //vRecoShowerClusterEff.clear();
    //vRecoShowerClusterPur.clear();
    //vRecoShowerClusterEffVsPur.clear();
    //hClusterHits      = nullptr;
    hMatchCorrectness = nullptr;

    fMMQualityTree = nullptr;
    
    _mcshowerTot    = 0;
    _recoClusterTot = 0;
  }

  bool MMQuality::initialize() {

    // at least 1 of the three (shower,pfpart,cluster) producer names needs to be specified
    if(fClusterProducer.empty() && fShowerProducer.empty() && fPFParticleProducer.empty()) {
      print(msg::kERROR,__FUNCTION__,"Cluster, PFParticle & Shower producer's name are all empty!");
      return false;
    }

    // only one of the three (shower,pfpart,cluster) producer names need to be specified
    int producers_set = 0;
    if ( !fClusterProducer.empty() )
      producers_set += 1;
    if ( !fShowerProducer.empty() )
      producers_set += 1;
    if ( !fPFParticleProducer.empty() )
      producers_set += 1;
    if (producers_set != 1){
      print(msg::kERROR,__FUNCTION__,"Please sepcify producer name for only one data-product amongst (Cluster,PFParticle,Shower)");
      return false;
    }

    // auto geo = larutil::Geometry::GetME();

    //
    // Cluster purity, efficiency histograms initialization
    //

    //for(auto& h : vShowerClusterEff) delete h;
    //for(auto& h : vShowerClusterPur) delete h;
    //for(auto& h : vShowerClusterEffVsPur) delete h;
    //for(auto& h : vRecoShowerClusterEff) delete h;
    //for(auto& h : vRecoShowerClusterPur) delete h;
    //for(auto& h : vRecoShowerClusterEffVsPur) delete h;

    //vShowerClusterEff.clear();
    //vShowerClusterPur.clear();
    //vShowerClusterEffVsPur.clear();
    //vRecoShowerClusterEff.clear();
    //vRecoShowerClusterPur.clear();
    //vRecoShowerClusterEffVsPur.clear();

    /*
    for(size_t plane=0; plane<geo->Nplanes(); ++plane) {

      vShowerClusterEff.push_back(new TH1D(Form("hShowerClusterEff_Plane%zu",plane),
					   "Best Shower Cluster's Charge Efficiency; Efficiency; Clusters",
					   101,-0.005,1.005));

      vShowerClusterPur.push_back(new TH1D(Form("hShowerClusterPur_Plane%zu",plane),
					   "Best Shower Cluster's Charge Purity; Purity; Clusters",
					   101,-0.005,1.005));

      vShowerClusterEffVsPur.push_back(new TH2D(Form("hShowerClusterEffVsPur_Plane%zu",plane),
						"Best Shower Cluster's Charge Efficiency vs Purity; Efficiency ; Purity",
						101,-0.005,1.005,101,-0.005,1.005));


      vRecoShowerClusterEff.push_back(new TH1D(Form("hRecoShowerClusterEff_Plane%zu",plane),
					       "Shower Cluster's Charge Efficiency; Efficiency; Clusters",
					       101,-0.005,1.005));
      
      vRecoShowerClusterPur.push_back(new TH1D(Form("hRecoShowerClusterPur_Plane%zu",plane),
					       "Shower Cluster's Charge Purity; Purity; Clusters",
					       101,-0.005,1.005));
      
      vRecoShowerClusterEffVsPur.push_back(new TH2D(Form("hRecoShowerClusterEffVsPur_Plane%zu",plane),
						    "Shower Cluster's Charge Efficiency vs Purity; Efficiency ; Purity",
						    101,-0.005,1.005,101,-0.005,1.005));
      

    }
    */
    
    //
    // Matching correctness histogram initialization
    //
    if(hMatchCorrectness) delete hMatchCorrectness;
    hMatchCorrectness = new TH1D("hMatchCorrectness",
				 "Shower 2D Cluster Matching Correctness; Correctness; Showers",
				 101,-0.005,1.005);

    //
    // Matching correctness histogram initialization
    //
    /*                                              
    if(hClusterHits) delete hClusterHits;
    hClusterHits = new TH1D("hClusterHits",
			    "Number of hits per 2D cluster; Number of hits; Counts",
                                 401,-0.5,400.5);
    */

    InitializeAnaTree();

    return true;
  }
  
  bool MMQuality::analyze(storage_manager* storage) {

    //Let's start by cleaning up the analysis TTree
    ClearTreeVar();
    
    //event_shower* ev_shower = nullptr;

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
    
    // Retrieve shower data product
    event_shower* ev_shower = nullptr;
    // if no shower producer specified -> don't bother to get the shower data-product
    if(!fShowerProducer.empty()) {
      ev_shower = storage->get_data<event_shower>(fShowerProducer);
      if(!ev_shower) {
	print(msg::kERROR,__FUNCTION__,Form("Did not find shower produced by \"%s\"",fShowerProducer.c_str()));
	return false;
      }
    }

    // Retrieve pfparticle data product
    event_pfpart* ev_pfpart = nullptr;
    // if no shower producer specified -> don't bother to get the shower data-product
    if(!fPFParticleProducer.empty()) {
      ev_pfpart = storage->get_data<event_pfpart>(fPFParticleProducer);
      if(!ev_pfpart) {
	print(msg::kERROR,__FUNCTION__,Form("Did not find PFParticle produced by \"%s\"",fPFParticleProducer.c_str()));
	return false;
      }
    }
    
    // Retrieve cluster data-product
    event_cluster* ev_cluster = nullptr;
    // association vector
    AssSet_t ass_cluster_v;

    // if a shower data-product was loaded -> get the clusters associated w/ the showers
    if(ev_shower) {
      ass_cluster_v = storage->find_one_ass(ev_shower->id(),ev_cluster,ev_shower->name());
      if (!ev_cluster || (!ev_shower->empty() && ass_cluster_v.empty())){
	print(msg::kERROR,__FUNCTION__,Form("No associated cluster found to a shower produced by \"%s\"",fShowerProducer.c_str()));
	return false;
      }
    }
    // if a PFParticle data-product was loaded -> load the clusters through this association
    else if (ev_pfpart) {
      ass_cluster_v = storage->find_one_ass(ev_pfpart->id(),ev_cluster,ev_pfpart->name());
      if (!ev_cluster || (!ev_pfpart->empty() && ass_cluster_v.empty())){
	print(msg::kERROR,__FUNCTION__,Form("No associated cluster found to a PFParticle produced by \"%s\"",fPFParticleProducer.c_str()));
	return false;
      }
    }
    // finally, if clusters are to be loaded directly from the producer name
    else{
      ev_cluster = storage->get_data<event_cluster>(fClusterProducer);
      if(!ev_cluster || !(ev_cluster->size())) {
	print(msg::kERROR,__FUNCTION__,"Could not retrieve a reconstructed cluster!");
	return false;
      }
    }

    // get associated hits
    event_hit* ev_hit = nullptr;
    // association vector (to clusters)
    AssSet_t ass_hit_v;

    ass_hit_v = storage->find_one_ass(ev_cluster->id(),ev_hit,ev_cluster->name());
    if ( !ev_hit or (ev_hit->size() == 0) or (ass_hit_v.size() == 0) ){
      print(msg::kERROR,__FUNCTION__,Form("No associated hit found to a cluster produced by \"%s\"",ev_cluster->name().c_str()));
      return false;
    }

    /*
    // Retrieve shower => cluster association
    auto ass_cluster_v = ev_shower->association(ev_cluster->id());

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
    std::vector<std::vector<unsigned int> > g4_trackid_v;
    std::vector<unsigned int> mc_index_v;
    g4_trackid_v.reserve(ev_mcs->size());
    for(size_t mc_index=0; mc_index<ev_mcs->size(); ++mc_index) {
      auto const& mcs = (*ev_mcs)[mc_index];
      auto energy = mcs.DetProfile().E();
      if( _mc_energy_min < energy && energy < _mc_energy_max ) {
	g4_trackid_v.push_back(mcs.DaughterTrackID());
	mc_index_v.push_back(mc_index);
      }
    }

    if(g4_trackid_v.empty()) {
      print(msg::kWARNING,__FUNCTION__,"No MCShower above cut energy value. Skipping an event...");
      return true;
    }

    if(!fBTAlg.BuildMap(g4_trackid_v, *ev_simch, *ev_hit, ass_hit_v)) {
      print(msg::kERROR,__FUNCTION__,"Failed to build back-tracking map for MC...");
      return false;
    }
    
    auto geo = larutil::Geometry::GetME();
   
    // Fill cluster quality plots
    for(size_t cluster_index=0; cluster_index<ev_cluster->size();++cluster_index){
      
      double _purity = 0.;   double _efficiency = 0.; 
      
      for(size_t mcs_index=0; mcs_index<mc_index_v.size(); ++mcs_index) {
	
	auto const ep_clust = fBTAlg.ClusterEP(cluster_index,mcs_index);
	
	if( ep_clust.first==0 && ep_clust.second==0 )
	  continue;
	
	if (ep_clust.second > _purity){
	  _purity     = ep_clust.second;
	  _efficiency = ep_clust.first ;
	}
      } // End of loop on mc showers 	
      
      
      if(_purity&&_efficiency) {
	
	// get associated hits                                                                                                                                                     
	//event_hit* ev_hit = nullptr;
	//ass_hit_v = storage->find_one_ass(ev_cluster->id(),ev_hit,ev_cluster->name());
	if (!ev_hit)
	  {
	    print(msg::kERROR,__FUNCTION__,Form("No associated hit found to a cluster produced by \"%s\"",ev_cluster->name().c_str()));
	    continue;
	  }
	
	size_t pl = ev_hit->at(ass_hit_v[cluster_index][0]).View();
	if (!pl) 
	  {
	    fRecoEff_0.push_back(_efficiency);       fRecoPur_0.push_back(_purity);
	  }
	else if(pl == 1) 
	  {
	    fRecoEff_1.push_back(_efficiency);       fRecoPur_1.push_back(_purity);
	  }
	else if(pl == 2) 
	  {
	    fRecoEff_2.push_back(_efficiency);       fRecoPur_2.push_back(_purity);
	  }
	fRecoHit.push_back(ass_hit_v[cluster_index].size());	
      }
    } // End of loop on clusters
    

    for(size_t mcs_index=0; mcs_index<mc_index_v.size(); ++mcs_index) {
      
      for(size_t plane = 0; plane < geo->Nplanes(); ++plane) {
	
	auto const ep = fBTAlg.BestClusterEP(mcs_index,plane);
	
	if( ep.first==0 && ep.second==0 )
	  continue;
	
	//vShowerClusterEff[plane]->Fill(ep.first);
	//vShowerClusterPur[plane]->Fill(ep.second);
        //vShowerClusterEffVsPur[plane]->Fill(ep.first,ep.second);
	
	if (!plane)
          {
            fBestEff_0.push_back(ep.first);       fBestPur_0.push_back(ep.second);
          }
        else if(plane == 1)
          {
            fBestEff_1.push_back(ep.first);       fBestPur_1.push_back(ep.second);
          }
        else if(plane == 2)
          {
            fBestEff_2.push_back(ep.first);       fBestPur_2.push_back(ep.second);
          }
	
      } // End of loop on planes
    } // End of loop on mc clusters
    
    
    _mcshowerTot   += mc_index_v.size();
    _recoClusterTot+= ev_cluster->size();
    fMCShower.push_back(mc_index_v.size());
    fMMQualityTree->Fill();

    // if no shower / PFParticle producer was set
    // we have no information on cross-plane cluster
    // matches -> we cannot evaluate matching performance
    // -> return now
    if ( fShowerProducer.empty() and fPFParticleProducer.empty() )
      return true;

    for(size_t match_index=0; match_index < ass_cluster_v.size(); match_index++) {
      auto res = fBTAlg.MatchCorrectness(ass_cluster_v[match_index]);
      hMatchCorrectness->Fill(res.second);
    }
    
    ClearTreeVar();
    return true;
  }

  bool MMQuality::finalize() {
    
    if(_fout) {
      
      // Write cluster histograms if any entry made
      //if(vShowerClusterEff.size() && vShowerClusterEff[0]->GetEntries()) {

      //for(auto& h : vShowerClusterEff) h->Write();
      //for(auto& h : vShowerClusterPur) h->Write();
      // for(auto& h : vShowerClusterEffVsPur) h->Write();
      //}
      
      
      //for(auto& h : vRecoShowerClusterEff) h->Write();
      //for(auto& h : vRecoShowerClusterPur) h->Write();
      //for(auto& h : vRecoShowerClusterEffVsPur) h->Write();
      
      
      // Write shower histograms if any entry made
      if(hMatchCorrectness->GetEntries()) hMatchCorrectness->Write();
      //if(hClusterHits->GetEntries()) hClusterHits->Write();

      // Write Ttree histograms if any entry made                                                                                                                                 
      if (fMMQualityTree) fMMQualityTree->Write();
    }
   
    std::cout<<"Total number of MCShowers    : "<< _mcshowerTot<<"\n";
    std::cout<<"Total number of RecoClusters : "<< _recoClusterTot<<"\n";

    return true;
  }

  void MMQuality::InitializeAnaTree()
  {
    if(fMMQualityTree) delete fMMQualityTree;

    fMMQualityTree = new TTree("fMMQualityTree","");

    fMMQualityTree->Branch("recoEff_0","vector<double>", &fRecoEff_0);
    fMMQualityTree->Branch("recoEff_1","vector<double>", &fRecoEff_1);
    fMMQualityTree->Branch("recoEff_2","vector<double>", &fRecoEff_2);
							            
    fMMQualityTree->Branch("recoPur_0","vector<double>", &fRecoPur_0);
    fMMQualityTree->Branch("recoPur_1","vector<double>", &fRecoPur_1);
    fMMQualityTree->Branch("recoPur_2","vector<double>", &fRecoPur_2);
							            
    fMMQualityTree->Branch("BestEff_0","vector<double>", &fBestEff_0);
    fMMQualityTree->Branch("BestEff_1","vector<double>", &fBestEff_1);
    fMMQualityTree->Branch("BestEff_2","vector<double>", &fBestEff_2);
							            
    fMMQualityTree->Branch("BestPur_0","vector<double>", &fBestPur_0);
    fMMQualityTree->Branch("BestPur_1","vector<double>", &fBestPur_1);
    fMMQualityTree->Branch("BestPur_2","vector<double>", &fBestPur_2);

    fMMQualityTree->Branch("RecoHits" ,"vector<int>",&fRecoHit);
    fMMQualityTree->Branch("MCShower" ,"vector<int>",&fMCShower);

  }

  void MMQuality::ClearTreeVar(){
    fMCShower.clear(); 
    fRecoHit.clear();
 
    fRecoEff_0.clear();
    fRecoEff_1.clear();
    fRecoEff_2.clear();
              
    fRecoPur_0.clear();
    fRecoPur_1.clear();
    fRecoPur_2.clear();
              
    fBestEff_0.clear();
    fBestEff_1.clear();
    fBestEff_2.clear();
              
    fBestPur_0.clear();
    fBestPur_1.clear();
    fBestPur_2.clear();

  }
}
#endif
