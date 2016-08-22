/**
 * \file ProtoShowerAlgOpenCV.h
 *
 * \ingroup ProtoShower
 *
 * \brief Class def header for a class ProtoShowerAlgOpenCV
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGOPENCV_H
#define PROTOSHOWERALGOPENCV_H

#include <iostream>

#include "ProtoShowerAlgBase.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

/**
   \class ProtoShowerAlgOpenCV
   User defined class ProtoShowerAlgOpenCV ... these comments are used to generate
   doxygen documentation!
 */

namespace protoshower {

class ProtoShowerAlgOpenCV : public ProtoShowerAlgBase {

public:

  /// Default constructor
  ProtoShowerAlgOpenCV();

  /// Default destructor
  ~ProtoShowerAlgOpenCV() {}

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

