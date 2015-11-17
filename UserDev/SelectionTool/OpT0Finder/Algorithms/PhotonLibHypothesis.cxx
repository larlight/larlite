#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"

namespace flashana {

  void PhotonLibHypothesis::FillEstimate(const QCluster_t& trk, Flash_t &flash_t)
  {

    size_t n_pmt = BaseAlgorithm::NOpDets();

    for ( auto& v : flash_t ) v = 0;

    for ( size_t ipmt = 0; ipmt < n_pmt; ++ipmt) {

      for ( size_t ipt = 0; ipt < trk.size(); ++ipt) {

        auto const& pt = trk[ipt];

        double q = pt.q;
        q *= ::phot::PhotonVisibilityService::GetME().GetVisibility( pt.x, pt.y, pt.z, ipmt);
        flash_t[ipmt] += q;

      }
    }

    double qsum = 0;
    for ( auto const& v : flash_t ) qsum += v;
    for ( auto& v : flash_t ) v/= qsum;

    return;
  }
}
#endif
