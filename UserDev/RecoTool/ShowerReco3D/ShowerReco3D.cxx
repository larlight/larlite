#ifndef RECOTOOL_SHOWERRECO3D_CXX
#define RECOTOOL_SHOWERRECO3D_CXX

#include "ShowerReco3D.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/DataFormat-TypeDef.h"
namespace larlite {

  ShowerReco3D::ShowerReco3D() : ana_base()
  {
    _name="ShowerReco3D";
    fInputProducer  = "fuzzycluster";
    fOutputProducer = "showerreco";
    fUsePFParticle  = false;
  }
  
  bool ShowerReco3D::initialize() {
    if(!fManager.Algo()) {
      
      throw ::cluster::CRUException("Shower reco algorithm not attached... aborting.");
      return false;

    }
    return true;
  }
  
  bool ShowerReco3D::analyze(storage_manager* storage) {

    // Re-initialize tools
    fManager.Reset();

    // Retrieve clusters and fed into the algorithm
    std::vector<std::vector<larutil::PxHit> > local_clusters;
    fCRUHelper.GeneratePxHit(storage,fInputProducer,local_clusters);

    // Create output data product holder
    auto shower_v = storage->get_data<event_shower>(fOutputProducer);
    shower_v->clear();
    shower_v->set_event_id(storage->get_data<event_cluster>(fInputProducer)->event_id());
    shower_v->set_run(storage->get_data<event_cluster>(fInputProducer)->run());
    shower_v->set_subrun(storage->get_data<event_cluster>(fInputProducer)->subrun());

    // Run matching
    std::vector<std::vector<unsigned int> > matched_pairs;
    if(!fUsePFParticle) {

      matched_pairs = fManager.Reconstruct(local_clusters,*shower_v);

    }else{

      auto pfpart_v = storage->get_data<event_pfpart>(fInputProducer);

      if(pfpart_v && pfpart_v->size()) {
      
	auto const& ass_clusters = pfpart_v->association(data::kCluster,
							 fInputProducer);
	// Loop over pfparticles
	for(size_t pfpart_index=0; pfpart_index<pfpart_v->size(); ++pfpart_index) {
	  
	  auto const& pfp = (*pfpart_v)[pfpart_index];
	  
	  // If this is not pdg=11 pfparticle, ignore
	  if(pfp.PdgCode()!=11) continue;
	  
	  // Otherwise we store the association information in matched_pair
	  matched_pairs.push_back(std::vector<unsigned int>());
	  
	  auto& last_pair = (*matched_pairs.rbegin());
	  
	  last_pair.reserve(ass_clusters[pfpart_index].size());
	  
	  // Loop over associated index for *this* pfparticle
	  for(auto const& ass_index : ass_clusters[pfpart_index]) 
	    last_pair.push_back(ass_index);
	}
      }

      fManager.Reconstruct(local_clusters,
			   matched_pairs,
			   *shower_v);
    }
	
    // Make sure result has the same size 
    if(shower_v->size() != matched_pairs.size())
      throw ::showerreco::ShowerRecoException("Mismatch in # of showers from algorithm's return!");

    for(size_t index = 0; index < shower_v->size(); ++index)

      (*shower_v)[index].set_id(index);
    
    shower_v->set_association(data::kCluster,fInputProducer,matched_pairs);
    return true;
  }
  
  bool ShowerReco3D::finalize() {

    return true;
  }
  
}
#endif
