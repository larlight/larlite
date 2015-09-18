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
#include "ClusterParams.h"
#include "CRUException.h"


namespace cluster {

  /**
     \class CRUHelper
     This class bridges the framework and ClusterRecoUtil package by being an interface
     to the framework's data products. Its mere purpose is to generate cluster_params
     instances from hit lists (fmwk data products).
  */
  class CRUHelper{
    
  public:
    
    /// Default constructor
    CRUHelper(){};
    
    /// Default destructor
    virtual ~CRUHelper(){};

    /// Generate: from 1 set of hits => 1 Params using indexes (association)
    void GenerateParams(const std::vector<unsigned int>& hit_index,
                        const larlite::event_hit* hits,
                        cluster_params &Params) const;
    
    /// Generate: Params vector from event storage by specifying cluster type
    void GenerateParams(::larlite::storage_manager* storage,
                        const std::string &cluster_producer_name,
                        std::vector<cluster::cluster_params> &Params_v) const;
    
    /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
    void GenerateHit2D(const std::vector<unsigned int>& hit_index,
                       const larlite::event_hit* hits,
                       std::vector<Hit2D> &pxhits) const;

    /// Generate: vector of PxHit sets from event storage by specifying cluster type
    void GenerateHit2D(larlite::storage_manager* storage,
                       const std::string &cluster_producer_name,
                       std::vector<std::vector<Hit2D> > &pxhits_v) const;
    
  };

}

#endif
/** @} */ // end of doxygen group 

