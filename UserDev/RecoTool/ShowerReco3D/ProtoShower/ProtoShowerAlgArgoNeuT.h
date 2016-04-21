/**
 * \file ProtoShowerAlgArgoNeuT.h
 *
 * \ingroup ProtoShower
 *
 * \brief Class def header for a class ProtoShowerAlgArgoNeuT
 *
 * @author Corey Adams
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGARGONEUT_H
#define PROTOSHOWERALGARGONEUT_H

#include <iostream>

#include "ProtoShowerAlgBase.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
// #include "Cluster3DRecoUtil/Base/CRU3DHelper.h"

// #include "Cluster3DRecoUtil/Alg/Default3DParamsAlg.h"

#include "ClusterRecoUtil/Alg/GetAverages.h"
#include "ClusterRecoUtil/Alg/FillPolygon.h"
#include "ClusterRecoUtil/Alg/GetRoughStartPoint.h"
#include "ClusterRecoUtil/Alg/InformedStartPoint.h"
#include "ClusterRecoUtil/Alg/RefineStartPoints.h"
#include "ClusterRecoUtil/Alg/SelectStartPoint.h"
#include "ClusterRecoUtil/Alg/FindShoweringPoint.h"
#include "ClusterRecoUtil/Alg/FillGeomParams.h"


/**
   \class ProtoShowerAlgArgoNeuT
   User defined class ProtoShowerAlgArgoNeuT ... these comments are used to generate
   doxygen documentation!
 */

namespace protoshower {

class ProtoShowerAlgArgoNeuT : ProtoShowerAlgBase {

public:

  /// Default constructor
  ProtoShowerAlgArgoNeuT();

  /// Default destructor
  ~ProtoShowerAlgArgoNeuT(){}

  void GenerateProtoShower(::larlite::storage_manager* storage,
                           ::larlite::event_pfpart* ev_pfpart,
                           const size_t proto_shower_pfpart,
                           protoshower::ProtoShower & proto_shower);


  // void SetClusterParamsAlg(::cluster::ClusterParamsAlg * _new_params_alg);
  // void SetCluster3DParamsAlg(::cluster3D::Cluster3DParamsAlg * _new_params3D_alg);

private:

  void init_params();

  void fill_params(::cluster::cluster_params &, Point2D );

  // ::cluster3D::Cluster3DParamsAlg * _params3D_alg;
  // ::cluster::ClusterParamsAlg * _params_alg;

  // Use the CRU Helpers to build cluster params:
  // ::cluster3D::CRU3DHelper _cru3D_helper;
  ::cluster::CRUHelper _cru_helper;

  // These are the cluster_params filling modules this will use:
  ::cluster::GetAverages        * averages;
  ::cluster::FillPolygon        * polygon;
  ::cluster::InformedStartPoint * informedStartPoint;
  ::cluster::GetRoughStartPoint * roughStartPoint;
  ::cluster::FindShoweringPoint * showeringpoint;
  ::cluster::SelectStartPoint   * selectStartPoint;
  ::cluster::RefineStartPoints  * refineStartPoint;
  ::cluster::FillGeomParams     * fillGeomParams;


};

}// namespace

#endif
/** @} */ // end of doxygen group

