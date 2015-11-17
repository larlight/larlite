#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"

namespace flashana {

  PhotonLibHypothesis* PhotonLibHypothesis::_me = nullptr;

  PhotonLibHypothesis::PhotonLibHypothesis()
    : _pmt_x_v()
    , _pmt_y_v()
    , _pmt_z_v()
  {}

  void PhotonLibHypothesis::SetOpDetPositions( const std::vector<double>& pos_x,
                                               const std::vector<double>& pos_y,
                                               const std::vector<double>& pos_z )
  {
    if(pos_x.size() != pos_y.size() || pos_x.size() != pos_z.size() )
      throw OpT0FinderException("Unmatching optical detector position array length!");
    _pmt_x_v = pos_x;
    _pmt_y_v = pos_y;
    _pmt_z_v = pos_z;

  }

  const std::vector<double>& PhotonLibHypothesis::ChargeHypothesis(const QCluster_t& trk,
                                                                   const double xshift)
  {

    double qmax = 0;
    if(_pmt_x_v.empty()) Print(msg::kEXCEPTION,__FUNCTION__,"PMT Geometry not set!");
    if(_qll_hypothesis_v.empty()) _qll_hypothesis_v.resize(_pmt_x_v.size(),0);
    if(_qll_hypothesis_v.size() != _pmt_x_v.size())
      Print(msg::kEXCEPTION,__FUNCTION__,"Hypothesis vector length != PMT count");

    for(auto& v : _qll_hypothesis_v) v=0;

    for(size_t pmt_index=0; pmt_index < _pmt_x_v.size(); ++pmt_index) {

      for(size_t pt_index=0; pt_index < trk.size(); ++pt_index) {

        auto const& pt = trk[pt_index];

        double q = pt.q;
        q *= ::phot::PhotonVisibilityService::GetME().GetVisibility((pt.x+xshift),pt.y,pt.z,pmt_index);
        _qll_hypothesis_v[pmt_index] += q;

      }

      if(_qll_hypothesis_v[pmt_index] > qmax) qmax = _qll_hypothesis_v[pmt_index];
    }

    //for(auto& v : _qll_hypothesis_v) v /= qmax;
    double qsum =0;
    for(auto const& v : _qll_hypothesis_v) qsum += v;
    for(auto& v : _qll_hypothesis_v) v/= qsum;

    return _qll_hypothesis_v;
  }
}
#endif
