#ifndef RECOTOOL_SHOWERRECO3D_CXX
#define RECOTOOL_SHOWERRECO3D_CXX

#include "ShowerReco3D.h"

namespace larlite {

  ShowerReco3D::ShowerReco3D() : ana_base(), fShowerAlgo(nullptr), fMatchMgr(nullptr)
  {
    _name="ShowerReco3D";
    fInputProducer  = "fuzzycluster";
    fOutputProducer = "showerreco";
    fUsePFParticle  = false;
    auto geom = ::larutil::Geometry::GetME();
    if(!fMatchMgr) fMatchMgr = new ::cmtool::CMatchManager(geom->Nplanes());
  }
  
  bool ShowerReco3D::initialize() {
    _mgr = 0;
    if(!fShowerAlgo) {
      
      throw ::cluster::CRUException("Shower reco algorithm not attached... aborting.");
      return false;

    }
    
    return true;
  }
  
  bool ShowerReco3D::analyze(storage_manager* storage) {

    _mgr = storage;
    // Re-initialize tools
    fShowerAlgo->Reset();
    fMatchMgr->Reset();

    // Retrieve clusters and fed into the algorithm
    std::vector<std::vector<larutil::PxHit> > local_clusters;

    fCRUHelper.GeneratePxHit(storage,fInputProducer,local_clusters);

    // Run matching
    std::vector<std::vector<unsigned int> > matched_pairs;
    if(!fUsePFParticle) {

      fMatchMgr->SetClusters(local_clusters);
      
      // Run matching & retrieve matched cluster indices
      try{
	fMatchMgr->Process();
      }catch( ::cmtool::CMTException &e){
	e.what();
	return false;
      }

      matched_pairs = fMatchMgr->GetBookKeeper().GetResult();

    }else{

      auto pfpart_v = storage->get_data<event_pfpart>(fInputProducer);

      auto const& ass_clusters = pfpart_v->association(data::kCluster,
						       fInputProducer);
      // Loop over pfparticles
      for(size_t pfpart_index=0; pfpart_index<pfpart_v->size(); ++pfpart_index) {

	auto const& pfp = pfpart_v[pfpart_index];

	// If this is not pdg=11 pfparticle, ignore
	if(pfp.PdgCode()!=11) continue;

	// Otherwise we store the association information in matched_pair
	matched_pairs.push_back(std::vector<unsigned int>());

	auto& last_pair = (*matched_pairs.rbegin());

	last_pair.reserve(ass.size());

	// Loop over associated index for *this* pfparticle
	for(auto const& ass_index : ass_clusters[pfpart_index]) 
	  last_pair.push_back(ass_index);
	
      }
    }

    // Create output data product holder
    auto shower_v = storage->get_data<event_shower>(fOutputProducer);
    shower_v->clear();
    shower_v->reserve(matched_pairs.size());
    shower_v->set_event_id(storage->get_data<event_cluster>(fInputProducer)->event_id());
    shower_v->set_run(storage->get_data<event_cluster>(fInputProducer)->run());
    shower_v->set_subrun(storage->get_data<event_cluster>(fInputProducer)->subrun());
    if(!matched_pairs.size()) return true;

    //
    // Reaching this point means there's some showers to reconstruct!
    // 1) Create association to be stored in an output
    // 2) Append input clusters to shower algo based on matched_pairs vector
    //
    AssSet_t ass_index_v;
    ass_index_v.reserve(matched_pairs.size());
    for(auto const& pair : matched_pairs) {
      
      std::vector< ::cluster::ClusterParamsAlg> cpans;
      
      cpans.reserve(pair.size());

      // Create an association vector
      AssUnit_t ass_index;
      ass_index.reserve(pair.size());
      
      for(auto const& index : pair) {

	ass_index.push_back(index);

	if(fUsePFParticle) {
	  
	  auto cluster_v = storage->get_data<event_cluster>(fInputProducer);

	  if(!cluster_v) 
	    throw ::showerreco::ShowerRecoException("Cannot find input cluster data!");
	  
	  ::cluster::ClusterParamsAlg cpan(local_clusters[index]);
	  cpan.SetVerbose(false);
	  cpan.SetPlane((*cluster_v)[index].View());
	  cpan.DisableFANN();
	  cpan.FillParams(false,false,false,false,false,false);
	  cpan.FillPolygon();
	  cpans.push_back(cpan);

	}else 
	  
	  cpans.push_back(fMatchMgr->GetInputClusters()[index]);
      }

      ass_index_v.push_back(ass_index);
      std::cout << "cpans.size(): " << cpans.size() << std::endl;
      for ( auto const& cpan : cpans ) {
        std::cout << "NHits: " << cpan.GetNHits() << std::endl;
      }
      fShowerAlgo->AppendInputClusters(cpans);
    }

    // Run shower reco
    auto result_v = fShowerAlgo->Reconstruct();

    // Make sure result has the same size 
    if(result_v.size() != ass_index_v.size())
      throw ::showerreco::ShowerRecoException("Mismatch in # of showers from algorithm's return!");

    for(size_t index = 0; index < result_v.size(); ++index) {

      auto& result = result_v.at(index);

      // Set ID
      result.set_id(shower_v->size());

      // Store
      shower_v->push_back(result);

    } // done looping over matched cluster pairs
    
    shower_v->set_association(data::kCluster,fInputProducer,ass_index_v);
    return true;
  }
  
  bool ShowerReco3D::finalize() {

    return true;
  }
  
}
#endif
