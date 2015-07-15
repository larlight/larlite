#ifndef SHOWERRECOMANAGER_CXX
#define SHOWERRECOMANAGER_CXX

#include "ShowerRecoManager.h"

namespace showerreco {

  ShowerRecoManager::ShowerRecoManager()
    : fMatchMgr(nullptr)
  {
    auto geom = ::larutil::Geometry::GetME();
    fMatchMgr = new ::cmtool::CMatchManager(geom->Nplanes());
  }

  void ShowerRecoManager::Reset()
  {
    for(auto alg : _alg_v) alg->Reset();

    fMatchMgr->Reset();
  }

  ClusterAss_t ShowerRecoManager::Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
					       std::vector<showerreco::Shower_t>& showers)
  {
    showers.clear();
    fMatchMgr->SetClusters(clusters);

    ClusterAss_t res_ass;
    // Run matching & retrieve matched cluster indices
    try{
      fMatchMgr->Process();
    }catch( ::cmtool::CMTException &e){
      e.what();
      return res_ass;
    }
    res_ass = fMatchMgr->GetBookKeeper().GetResult();
  
    Process(res_ass, showers);

    return res_ass;
  }
  
  void ShowerRecoManager::Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
				       const ClusterAss_t& ass,
				       std::vector<showerreco::Shower_t>& showers)
  {
    showers.clear();
    fMatchMgr->SetClusters(clusters);
    Process(ass,showers);
  }
  
  void ShowerRecoManager::Process(const ClusterAss_t& ass,
				  std::vector<showerreco::Shower_t>& showers)
  {

    showers.clear();
    showers.reserve(ass.size());
    for(auto const& pair : ass) {

      ShowerClusterSet_t in_clusters;
      in_clusters.reserve(pair.size());
      
      for(auto const& index : pair)

	in_clusters.emplace_back( cpan_2_scluster (fMatchMgr->GetInputClusters()[index]) );

      for(auto& shower_alg : _alg_v)

	showers.emplace_back(shower_alg->RecoOneShower(in_clusters));

      for(auto& shower_ana : _ana_v)

	shower_ana->Analyze(showers.back(),in_clusters);
	
    }

  }

  void ShowerRecoManager::Finalize(TFile* fout){
    if (!fout)
      return;
    fout->cd();
    for(auto& ana : _ana_v) {
      auto tree = ana->GetTree();
      if (tree) tree->Write();
    }
    return;
  }
  
}

#endif
