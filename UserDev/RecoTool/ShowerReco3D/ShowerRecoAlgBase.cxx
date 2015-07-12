#ifndef RECOTOOL_SHOWERRECOALGBASE_CXX
#define RECOTOOL_SHOWERRECOALGBASE_CXX

#include "ShowerRecoAlgBase.h"

namespace showerreco {
  
  ShowerRecoAlgBase::ShowerRecoAlgBase()
    : fInputClusters()
    , _alg_tree(nullptr)
  {
    fCaloAlg   = nullptr;
    fVerbosity = false;
    _linearE   = false;
  }

  void ShowerRecoAlgBase::Reset()
  {
    fInputClusters.clear();
  }

  void ShowerRecoAlgBase::AppendInputClusters(const std::vector< ::cluster::ClusterParamsAlg>& cpan_v)
  {
    std::vector< ::showerreco::ShowerCluster_t> clusters;
    clusters.reserve(cpan_v.size());

    for(auto const& cpan : cpan_v) {

      clusters.push_back( ::showerreco::ShowerCluster_t() );

      (*clusters.rbegin()).start_point      = cpan.GetParams().start_point;
      (*clusters.rbegin()).end_point        = cpan.GetParams().end_point;
      (*clusters.rbegin()).angle_2d         = cpan.GetParams().angle_2d;
      (*clusters.rbegin()).opening_angle    = cpan.GetParams().opening_angle;
      (*clusters.rbegin()).plane_id         = cpan.Plane();
      (*clusters.rbegin()).hit_vector       = cpan.GetHitVector();

    }

    fInputClusters.push_back(clusters);

  }

  std::vector< ::larlite::shower> ShowerRecoAlgBase::Reconstruct()
  {

    if(!fCaloAlg) throw ShowerRecoException("Calorimetry algorithm must be provided!");

    ProcessInputClusters();

    std::vector< ::larlite::shower> output;

    output.reserve(fInputClusters.size());

    for(auto const& clusters : fInputClusters)

      output.push_back( RecoOneShower(clusters) );

    return output;

  }



}

#endif
