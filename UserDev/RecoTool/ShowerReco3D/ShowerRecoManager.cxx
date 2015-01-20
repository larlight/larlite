#ifndef SHOWERRECOMANAGER_CXX
#define SHOWERRECOMANAGER_CXX

#include "ShowerRecoManager.h"

namespace showerreco {

  ShowerRecoManager::ShowerRecoManager()
    : fShowerAlgo(nullptr)
    , fMatchMgr(nullptr)
  {
    fMatch    = true;
    auto geom = ::larutil::Geometry::GetME();
    fMatchMgr = new ::cmtool::CMatchManager(geom->Nplanes());
  }

  void ShowerRecoManager::Reset()
  {
    if(fShowerAlgo) fShowerAlgo->Reset();
    fMatchMgr->Reset();
  }

  ClusterAss_t ShowerRecoManager::Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
					       std::vector< ::larlite::shower>& showers)
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
				       std::vector< ::larlite::shower>& showers)
  {
    showers.clear();
    fMatchMgr->SetClusters(clusters);

    Process(ass,showers);
  }
  
  void ShowerRecoManager::Process(const ClusterAss_t& ass,
				  std::vector< ::larlite::shower>& showers)
  {
    
    for(auto const& pair : ass) {
      
      std::vector< ::cluster::ClusterParamsAlg> cpans;
      
      cpans.reserve(pair.size());
      
      for(auto const& index : pair)

	cpans.push_back(fMatchMgr->GetInputClusters()[index]);

      fShowerAlgo->AppendInputClusters(cpans);
    }

    // Run shower reco
    showers = fShowerAlgo->Reconstruct();
  }

}

#endif
