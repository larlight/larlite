#ifndef SHOWERRECOMANAGER_CXX
#define SHOWERRECOMANAGER_CXX

#include "ShowerRecoManager.h"
#include <iomanip>

namespace showerreco {

ShowerRecoManager::ShowerRecoManager()
{
  _CPAN_times = 0.;
  _CPAN_calls = 0;
}

void ShowerRecoManager::Initialize()
{
  for (auto alg : _alg_v)
    alg->Initialize();

  return;
}

  void ShowerRecoManager::Reset()
  {
    for (auto alg : _alg_v) alg->Reset();
    
    _clusters.clear();
    
  }
  
  void ShowerRecoManager::Reconstruct (const ClusterAss_t& ass,
				       std::vector<showerreco::Shower_t>& showers)
  {

    showers.clear();
    showers.reserve(ass.size());
    
    for (auto const& pair : ass) {
      
      ShowerClusterSet_t in_clusters;
      in_clusters.reserve(pair.size());
      
      for (auto const& index : pair) {
	in_clusters.emplace_back( _clusters[index] );
	// Make sure to fill the params:
	_watch.Start();
	_params_alg.FillParams(in_clusters.back());
	_CPAN_times += _watch.RealTime();
	_CPAN_calls += 1;
      }
      
      for (auto& shower_alg : _alg_v){
	auto this_shower = shower_alg->RecoOneShower(in_clusters);
	showers.emplace_back(this_shower);
      }// for all shower reco algorithms? not modular algos...
      // why is this a for-loop? DC very confused here...
      for (auto& shower_ana : _ana_v)
	shower_ana->Analyze(showers.back(), in_clusters);
    }// for all association pairs    
    
    return;
  }
  
  void ShowerRecoManager::Finalize(TFile* fout) {
    if (!fout)
      return;
    fout->cd();
    
    // print out profiling performance for CPAN and matching
    double CPAN_T    = _CPAN_times / (double) _CPAN_calls;
    std::cout << std::endl
	      << "=================== Time Report =====================" << std::endl
	      << "CPAN Time    : " << std::setw(10) << CPAN_T * 1.e6 << " [us/cluster]" << std::endl
	      << "=====================================================" << std::endl
	      << std::endl;
    
    // loop through algorithms
    for (auto& alg : _alg_v)
      alg->Finalize(fout);
    
    // loop through ana modules
    for (auto& ana : _ana_v) {
      auto tree = ana->GetTree();
      if (tree) tree->Write();
    }
    
    return;
  }
  
}

#endif
