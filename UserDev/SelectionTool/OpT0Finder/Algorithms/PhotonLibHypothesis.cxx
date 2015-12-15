#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"

namespace flashana {

  PhotonLibHypothesis::PhotonLibHypothesis(const std::string name)
    : BaseFlashHypothesis(name)
  {}

  void PhotonLibHypothesis::Configure(const ::fcllite::PSet &pset)
  {}
  
  void PhotonLibHypothesis::FillEstimate(const QCluster_t& trk,
					 Flash_t &flash) const
  {
    
    size_t n_pmt = BaseAlgorithm::NOpDets();
    
    for ( auto& v : flash.pe_v ) v = 0;
    
    for ( size_t ipmt = 0; ipmt < n_pmt; ++ipmt) {

      for ( size_t ipt = 0; ipt < trk.size(); ++ipt) {
	
        auto const& pt = trk[ipt];
	
        double q = pt.q ;
        q *= ::phot::PhotonVisibilityService::GetME().GetVisibility( pt.x, pt.y, pt.z, ipmt);
        flash.pe_v[ipmt] += q;

	std::cout << "PhotonLibHypothesis: PMT : " << ipmt << " [x,y,z,q] -> [q] : [" << pt.x << ", "
		  << pt.y << ", " << pt.z << ", " << pt.q <<"] -> [" << q << "]" << std::endl;

      }
    }

    return;
  }
}
#endif
