#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
namespace flashana {

  static PhotonLibHypothesisFactory __global_PhotonLibHypothesisFactory__;

  PhotonLibHypothesis::PhotonLibHypothesis(const std::string name)
    : BaseFlashHypothesis(name)
  {}

  void PhotonLibHypothesis::_Configure_(const Config_t &pset)
  {
    _global_qe = pset.get<double>("GlobalQE");
    _qe_v      = pset.get<std::vector<double> >("CCVCorrection");
    _libraryfile = pset.get<std::string>("LibraryFile","");
    if (_libraryfile.empty())
      fLibrarySet = false;
    else
      fLibrarySet = true;
    if(_qe_v.size() != NOpDets()) {
      FLASH_CRITICAL() << "CCVCorrection factor array has size " << _qe_v.size()
		       << " != number of opdet (" << NOpDets() << ")!" << std::endl;
      throw OpT0FinderException();
    }
  }
  
  void PhotonLibHypothesis::FillEstimate(const QCluster_t& trk,
					 Flash_t &flash) const
  {
    
    size_t n_pmt = BaseAlgorithm::NOpDets();//n_pmt returns 0 now, needs to be fixed
    const ::phot::PhotonVisibilityService*  pPhotonLib = NULL;
    if ( fLibrarySet )
      pPhotonLib = &(::phot::PhotonVisibilityService::GetME(_libraryfile));
    else
      pPhotonLib = &(::phot::PhotonVisibilityService::GetME());
    
    for ( auto& v : flash.pe_v ) v = 0;
    
    for ( size_t ipmt = 0; ipmt < n_pmt; ++ipmt) {

      for ( size_t ipt = 0; ipt < trk.size(); ++ipt) {
	
        auto const& pt = trk[ipt];
	
        double q = pt.q;
	
        q *= pPhotonLib->GetVisibility( pt.x, pt.y, pt.z, ipmt) * _global_qe / _qe_v[ipmt];
        flash.pe_v[ipmt] += q;
	//std::cout << "PMT : " << ipmt << " [x,y,z] -> [q] : [" << pt.x << ", " << pt.y << ", " << pt.z << "] -> [" << q << std::endl;
	
      }
    }
    
    return;
  }
}
#endif
