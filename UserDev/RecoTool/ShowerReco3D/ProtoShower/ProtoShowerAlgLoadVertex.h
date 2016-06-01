/**
 * \file ProtoShowerAlgLoadVertex.h
 *
 * \ingroup ProtoShower
 *
 * \brief Class def header for a class ProtoShowerAlgLoadVertex
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGLOADVERTEX_H
#define PROTOSHOWERALGLOADVERTEX_H

#include <iostream>

#include "ProtoShowerAlgBase.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

/**
   \class ProtoShowerAlgLoadVertex
   User defined class ProtoShowerAlgLoadVertex ... these comments are used to generate
   doxygen documentation!
 */

namespace protoshower {

class ProtoShowerAlgLoadVertex : ProtoShowerAlgBase {

public:

  /// Default constructor
  ProtoShowerAlgLoadVertex();

  /// Default destructor
  ~ProtoShowerAlgLoadVertex() {}

  void GenerateProtoShower(::larlite::storage_manager* storage,
                           ::larlite::event_pfpart* ev_pfpart,
                           const size_t proto_shower_pfpart,
                           protoshower::ProtoShower & proto_shower);


private:

  ::cluster::ClusterParamsAlg * _params_alg;
  ::cluster::CRUHelper _cru_helper;

};

}// namespace

#endif
/** @} */ // end of doxygen group

