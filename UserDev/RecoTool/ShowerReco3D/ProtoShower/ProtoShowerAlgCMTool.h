/**
 * \file ProtoShowerAlgCMTool.h
 *
 * \ingroup ProtoShower
 *
 * \brief Class def header for a class ProtoShowerAlgCMTool
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGCMTOOL_H
#define PROTOSHOWERALGCMTOOL_H

#include <iostream>

#include "ProtoShowerAlgBase.h"

#include "ClusterRecoUtil/Base/CRUHelper.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"

/**
   \class ProtoShowerAlgCMTool
   User defined class ProtoShowerAlgCMTool ... these comments are used to generate
   doxygen documentation!
 */

namespace protoshower {

class ProtoShowerAlgCMTool : public ProtoShowerAlgBase {

public:

  /// Default constructor
  ProtoShowerAlgCMTool();

  /// Default destructor
  ~ProtoShowerAlgCMTool() {}

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

