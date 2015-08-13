/**
 * \file CRUHelper.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for a class CRUHelper
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef RECOTOOL_CRUHELPER_H
#define RECOTOOL_CRUHELPER_H

#include <iostream>
#include "DataFormat/storage_manager.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "ClusterParamsAlg.h"

namespace cluster {

  /**
     \class CRUHelper
     This class bridges the framework and ClusterRecoUtil package by being an interface
     to the framework's data products. Its mere purpose is to generate ClusterParamsAlg
     instances from hit lists (fmwk data products).
  */
  class CRUHelper{
    
  public:
    
    /// Default constructor
    CRUHelper(){};
    
    /// Default destructor
    virtual ~CRUHelper(){};

    /// Generate: from 1 set of hits => 1 CPAN using indexes (association)
    void GenerateCPAN(const std::vector<unsigned int>& hit_index,
		      const larlite::event_hit* hits,
		      ClusterParamsAlg &cpan) const;
    
    /// Generate: CPAN vector from event storage by specifying cluster type
    void GenerateCPAN(::larlite::storage_manager* storage,
		      const std::string &cluster_producer_name,
		      std::vector<cluster::ClusterParamsAlg> &cpan_v) const;
    
    /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
    void GeneratePxHit(const std::vector<unsigned int>& hit_index,
		       const larlite::event_hit* hits,
		       std::vector<larutil::PxHit> &pxhits) const;

    /// Generate: vector of PxHit sets from event storage by specifying cluster type
    void GeneratePxHit(larlite::storage_manager* storage,
		       const std::string &cluster_producer_name,
		       std::vector<std::vector<larutil::PxHit> > &pxhits_v) const;
    
  };

}

#endif
/** @} */ // end of doxygen group 

