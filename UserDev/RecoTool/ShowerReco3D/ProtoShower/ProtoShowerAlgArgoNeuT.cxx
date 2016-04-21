#ifndef PROTOSHOWERALGARGONEUT_CXX
#define PROTOSHOWERALGARGONEUT_CXX

#include "ProtoShowerAlgArgoNeuT.h"
#include "TVector3.h"

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/DetectorProperties.h"

#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/endpoint2d.h"
#include "DataFormat/vertex.h"

namespace showerreco {

ProtoShowerAlgArgoNeuT::ProtoShowerAlgArgoNeuT() {

  _name = "ProtoShowerAlgArgoNeuT";

  init_params();

}

void ProtoShowerAlgArgoNeuT::init_params() {

  // In the constructor, create, initialize and attach a set of algs
  averages              = new ::cluster::GetAverages();
  averages -> SetVerbose(false);

  polygon               = new ::cluster::FillPolygon();
  polygon -> SetVerbose(false);

  informedStartPoint    = new ::cluster::InformedStartPoint();
  informedStartPoint -> SetVerbose(false);

  roughStartPoint       = new ::cluster::GetRoughStartPoint();
  roughStartPoint -> SetVerbose(false);

  showeringpoint        = new ::cluster::FindShoweringPoint();
  showeringpoint -> SetVerbose(false);

  selectStartPoint      = new ::cluster::SelectStartPoint();
  selectStartPoint -> SetVerbose(false);

  refineStartPoint      = new ::cluster::RefineStartPoints();
  refineStartPoint -> SetVerbose(false);

  fillGeomParams        = new ::cluster::FillGeomParams();
  fillGeomParams -> SetVerbose(false);

}

void ProtoShowerAlgArgoNeuT::fill_params(
  ::cluster::cluster_params & params,
  Point2D start_point_guess)
{
  // Fill the params while suggesting a start point:

  averages -> do_params_fill(params);
  polygon -> do_params_fill(params);

  try {
    informedStartPoint -> set_start_point_guess(start_point_guess);
    informedStartPoint -> do_params_fill(params);
  }
  catch ( ... ) {
    std::cout << "Informed start point failed!" << std::endl;
    roughStartPoint -> do_params_fill(params);
  }


  showeringpoint -> do_params_fill(params);
  selectStartPoint -> do_params_fill(params);
  refineStartPoint -> do_params_fill(params);
  fillGeomParams -> do_params_fill(params);
}

// void ProtoShowerAlgArgoNeuT::SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg) {

//   if (_params_alg) {
//     delete _params_alg;
//   }
//   _params_alg = _new_params_alg;

//   return;
// }

// void ProtoShowerAlgArgoNeuT::SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg) {

//   if (_params3D_alg) {
//     delete _params3D_alg;
//   }
//   _params3D_alg = _new_params3D_alg;

//   return;
// }

void ProtoShowerAlgArgoNeuT::GenerateProtoShower(
  ::larlite::storage_manager* storage,
  ::larlite::event_pfpart* ev_pfpart,
  const size_t proto_shower_pfpart,
  showerreco::ProtoShower & proto_shower)
{


  auto geoHelper = larutil::GeometryHelper::GetME();
  auto geom = larutil::Geometry::GetME();
  auto detProp = larutil::DetectorProperties::GetME();


  // This class is an example of ProtoShowerAlg for argoneut's dEdx analysis
  // It uses clusters, which have endpoints associated to them
  //
  // So, it gets the clusters, their matching endpoints, and feeds them into
  // a custom version of cluster params


  // For each data product, fetch it and feed
  // it into the proto showers if possible
  //
  // That means using cluster_params and cluster3D_params if needed

  // Try to get the clusters if they are there
  ::larlite::event_cluster * ev_clust = nullptr;
  auto const& ass_cluster_v = storage->find_one_ass(ev_pfpart->id(), ev_clust, ev_pfpart->name());
  // These are the clusters associated with all the pfparticles, but we only care
  // about the pfparticle at index proto_shower_pfpart


  if (ev_clust && (ev_clust->size() != 0) ) {

    // Want to get the hits from the cluster too:
    ::larlite::event_hit * ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_clust->id(), ev_hit, ev_clust->name());


    // And get the endpoints associated with the clusters:
    ::larlite::event_endpoint2d * ev_endpoint = nullptr;
    auto const& ass_endpoint_v = storage->find_one_ass(ev_clust->id(), ev_endpoint, ev_clust->name());


    // For each cluster associated with this pfparticle, make the cluster_params
    // object (unfilled) using cru_helper

    if (ev_hit && (ev_hit->size() != 0) ) {
      // Loop over the clusters and initialize the params:
      for (auto & cluster_index : ass_cluster_v.at(proto_shower_pfpart) ) {
        proto_shower._params.push_back(::cluster::cluster_params());
        _cru_helper.GenerateParams(ass_hit_v.at(cluster_index),
                                   ev_hit,
                                   proto_shower._params.back());

        // Before filling the params, get the endpoint 2d associated with the cluster
        auto & _clust_endpoint = ev_endpoint->at(ass_endpoint_v.at(cluster_index).front());


        double * planeorigin;
        planeorigin = new double[geom->Nviews()];
        geom->PlaneOriginVtx(_clust_endpoint.View(), planeorigin);
        float planeOffset = planeorigin[0];

        delete planeorigin;

        // Get the endpoint if it has it:
        Point2D _end_point;
        _end_point.w = _clust_endpoint.Wire() * geoHelper ->WireToCm();
        _end_point.t =  (_clust_endpoint.DriftTime() - detProp->TriggerOffset())
                        * geoHelper->TimeToCm() + planeOffset;

        // Now we can fill the params
        fill_params(proto_shower._params.back(), _end_point);
      }
      proto_shower.hasCluster2D(true);
    }// if there are associated hits
    else
      proto_shower.hasCluster2D(false);
  }// if there are associated clusters
  else
    proto_shower.hasCluster2D(false);

  return;
}


}// namespace

#endif
