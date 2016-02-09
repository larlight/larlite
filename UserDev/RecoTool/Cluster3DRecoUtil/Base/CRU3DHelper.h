/**
 * \file CRU3DHelper.h
 *
 * \ingroup ClusterRecoUtil
 *
 * \brief Class def header for a class CRU3DHelper
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef RECOTOOL_CRUHELPER_H
#define RECOTOOL_CRUHELPER_H

#include <iostream>
#include "DataFormat/storage_manager.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/spacepoint.h"
#include "Cluster3DParams.h"
#include "CRU3DException.h"


namespace cluster3D {

/**
   \class CRU3DHelper
   This class bridges the framework and Cluster3DRecoUtil package by being an interface
   to the framework's data products. Its mere purpose is to generate cluster_params
   instances from hit lists (fmwk data products).
*/
class CRU3DHelper {

public:

  /// Default constructor
  CRU3DHelper() {};

  /// Default destructor
  virtual ~CRU3DHelper() {};

  /// Generate: from 1 set of sps => 1 Params3D using indexes (association)
  void GenerateParams3D(const std::vector<unsigned int>& sps_index,
                        const larlite::event_spacepoint* points,
                        cluster3D_params &params) const;

  /// Generate: Params vector from event storage by specifying pfpart type
  void GenerateParams3D(::larlite::storage_manager* storage,
                        const std::string &pfpart_producer_name,
                        std::vector<cluster3D_params> &params_v) const;

  /// Generate: from 1 set of sps => 1 set of Point3D using indexes (association)
  void GeneratePoint3D(const std::vector<unsigned int>& sps_index,
                       const larlite::event_spacepoint* hits,
                       std::vector<Point3D> &point3D_set) const;

  /// Generate: vector of Point3D sets from event storage by specifying pfpart type
  void GeneratePoint3D(larlite::storage_manager* storage,
                       const std::string &pfpart_producer_name,
                       std::vector<std::vector<Point3D> > &point3D_set_v) const;

};

}

#endif
/** @} */ // end of doxygen group

