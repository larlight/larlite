/**
 * \file ProtoShowerAlgPhotonRecluster.h
 *
 * \ingroup ProtoShower
 *
 * \brief Class def header for a class ProtoShowerAlgPhotonRecluster
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGPHOTONRECLUSTER_H
#define PROTOSHOWERALGPHOTONRECLUSTER_H

#include <iostream>

#include "ProtoShowerAlgBase.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

/**
   \class ProtoShowerAlgPhotonRecluster
   User defined class ProtoShowerAlgPhotonRecluster ... these comments are used to generate
   doxygen documentation!
 */

namespace protoshower {

class ProtoShowerAlgPhotonRecluster : public ProtoShowerAlgBase {

public:

  /// Default constructor
  ProtoShowerAlgPhotonRecluster();

  /// Default destructor
  ~ProtoShowerAlgPhotonRecluster() {}

  void GenerateProtoShower(::larlite::storage_manager* storage,
                           ::larlite::event_pfpart* ev_pfpart,
                           const size_t proto_shower_pfpart,
                           protoshower::ProtoShower & proto_shower);

  void SetVertexProducer(std::string s) { _vtx_producer = s; }


private:

  ::cluster::ClusterParamsAlg * _params_alg;
  ::cluster::CRUHelper _cru_helper;

  // vertex producer
  std::string _vtx_producer;

};

}// namespace

#endif
/** @} */ // end of doxygen group

