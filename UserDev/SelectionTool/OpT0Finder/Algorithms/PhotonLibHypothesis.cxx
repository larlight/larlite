#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"

namespace flashana {

  PhotonLibHypothesis::PhotonLibHypothesis(const std::string name)
    : BaseFlashHypothesis(name)
  {std::cout << "called constructor..." << std::endl;}

  void PhotonLibHypothesis::Configure(const ::fcllite::PSet &pset)
  {}
  
  void PhotonLibHypothesis::FillEstimate(const QCluster_t& trk,
					 Flash_t &flash) const
  {
    
    std::cout << "filling estimate..." << std::endl;
    std::cout << "number of track points: " << trk.size() << std::endl;
    std::cout<<"?????";
    size_t n_pmt = BaseAlgorithm::NOpDets();
    
    //size_t n_pmt = BaseAlgorithm::NOpDets();

    for ( auto& v : flash.pe_v ) v = 0;
    
    for ( size_t ipmt = 0; ipmt < 32; ++ipmt) {

      for ( size_t ipt = 0; ipt < trk.size(); ++ipt) {
	
        auto const& pt = trk[ipt];
	
        double q = pt.q;
	std::cout << "q @ pt " << ipt << " is " << q << std::endl;
        q *= ::phot::PhotonVisibilityService::GetME().GetVisibility( pt.x, pt.y, pt.z, ipmt);
	double q = pt.q;
	std::cout<<q;
        flash.pe_v[ipmt] += q;
	std::cout << "PMT : " << ipmt << " [x,y,z] -> [q] : [" << pt.x << ", "
			  << pt.y << ", " << pt.z << "] -> [" << q << std::endl;

      }
    }

    return;
  }
}
#endif
