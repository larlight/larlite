/**
 * \file ProtoShowerHelper.h
 *
 * \ingroup Base
 *
 * \brief Class def header for a class ProtoShowerHelper
 *
 * @author cadams
 */

/** \addtogroup Base

    @{*/
#ifndef PROTOSHOWERHELPER_H
#define PROTOSHOWERHELPER_H

#include <iostream>

#include "DataFormat/storage_manager.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/seed.h"
#include "DataFormat/vertex.h"

#include "ProtoShower.h"

#include "ProtoShowerAlgBase.h"

/**
   \class ProtoShowerHelper
   User defined class ProtoShowerHelper ... these comments are used to generate
   doxygen documentation!
 */
namespace protoshower {

class ProtoShowerHelper {

public:

  /// Default constructor
  ProtoShowerHelper();

  /// Default destructor
  ~ProtoShowerHelper() {}

  // From the storage manager, and with the pfpart producer name, generate
  // a vector of all the protoshowers in the event.
  void GenerateProtoShowers(::larlite::storage_manager* storage,
                            const std::string &pfpart_producer_name,
                            std::vector<::protoshower::ProtoShower> & proto_showers,
                            std::vector<unsigned int> showerLikePFParts = std::vector<unsigned int>(1, 999999));

  // set algorithm to use to create ProtoShowers from LArLite data-products
  void setProtoShowerAlg(ProtoShowerAlgBase *alg) { _proto_shower_alg = alg; }

private:

  // base class for Proto Shower algorithm
  ProtoShowerAlgBase *_proto_shower_alg;

};

} // protoshower


#endif
/** @} */ // end of doxygen group

