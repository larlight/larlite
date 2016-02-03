#ifndef LARLITE_CLUSTERINFO_CXX
#define LARLITE_CLUSTERINFO_CXX

#include "ClusterInfo.h"
#include "DataFormat/cluster.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

namespace showerreco {

  bool ClusterInfo::initialize() {
    
    if(_tree) delete _tree;
    _tree = new TTree(_name.c_str(),"Cluster params info");
    _tree->Branch("_fann","std::vector<float>",&_fann);
    std::cout << "done initialising!" << std::endl;
    return true;

  }
  
  bool ClusterInfo::analyze(larlite::storage_manager* storage) {

    // Get all of the clusters from this event:
    auto ev_clus = storage->get_data<larlite::event_cluster>(_input_producer);
    if (!ev_clus) {
      std::cout << "ev_clus is == 0, returning.\n";
      return false;
    }

    ::larlite::event_hit* ev_hit = nullptr;
    auto const& hit_index_v = storage->find_one_ass(ev_clus->id(), ev_hit, _input_producer);

    if (!ev_hit) {
      std::cout << "Did not find hit data product"
      << "!" << std::endl;
      return false;
    }
    
    
    if (!hit_index_v.size())
      return false;
    
    
    // std::cout << "reset  id is " << out_cluster_v -> event_id() << std::endl;
    
    if (!ev_clus->size()) {
      
      print(larlite::msg::kWARNING, __FUNCTION__,
            Form("Skipping event %d since no cluster found...", ev_clus->event_id()));
      return false;
    }
    
    // Loop over the clusters, and save only the ones that have potential to be a shower
    
    
    ::cluster::DefaultParamsAlg params_alg ;
    ::cluster::cluster_params params;
    params_alg.SetVerbose(false);
    params_alg.SetDebug(false);
    params_alg.SetMinHits(50);
    
    
    size_t index = 0;
    
    larlite::AssSet_t hit_ass;
    
    for (auto const& hit_indices : hit_index_v) {
      
      if (hit_indices.size() < 50) {
        index ++;
        continue;
      }

      // Fill the cluster params alg
      _cru_helper.GenerateParams( hit_indices, ev_hit, params);
      
      params_alg.FillParams(params);
      
      // ok let's test to see if this worked
      
      params.GetFANNVector(_fann);
      
      _tree->Fill();
    }
    
    return true;
  }

  bool ClusterInfo::finalize() {

    _fout->Write();
  
    return true;
  }

}
#endif
