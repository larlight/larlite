/**
 * \file ClusterMaker.h
 *
 * \ingroup CMToolBase
 * 
 * \brief Class def header for a class Cluster
 *
 * @author david caratelli
 */

/** \addtogroup CMToolBase

    @{*/
#ifndef CLUSTERMAKER_H
#define CLUSTERMAKER_H

#include <iostream>
#include <vector>

#include "DataFormat/storage_manager.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/vertex.h"

#include "Cluster.h"

/**
   \class Cluster
   User defined class Cluster ... these comments are used to generate
   doxygen documentation!
 */

namespace cluster {


  
  class ClusterMaker {
    
  public:
    
    /// Default constructor
    ClusterMaker();
    
    /// Default destructor
    ~ClusterMaker(){}

    void MakeClusters(const std::vector<std::vector<const larlite::hit*> >& hit_vv,
		      const larlite::vertex& vertex,
		      std::vector<cluster::Cluster>& cluster);
    
    void MakeClusters(::larlite::storage_manager* storage,
		      const std::vector< std::vector<unsigned int> >& clus_hit_indexes,
		      const std::string& clus_producer,
		      const std::string& vtx_producer,
		      std::vector<cluster::Cluster>& cluster);
    
    void MakeClusters(::larlite::storage_manager* storage,
		      const std::string& clus_producer,
		      const std::string& vtx_producer,
		      std::vector<cluster::Cluster>& cluster);
    
  private:

    bool loadVertex(::larlite::event_vertex *ev_vtx);
    bool loadVertex(const larlite::vertex& vertex);

    void GetClusterPts(const ::larlite::event_hit* ev_hit,
		       const std::vector<unsigned int>& hit_idx_v,
		       std::vector<::cluster::pt>& pt_v);
    
    void GetClusterPts(const std::vector<const ::larlite::hit*>& hit_v,
		       std::vector<::cluster::pt>& pt_v);

    /// vertex coordinates
    std::vector<double> _vtx_w_cm;
    std::vector<double> _vtx_t_cm;
    
    /// conversion factors for hits
    double _wire2cm, _time2cm;

  };

}
#endif
/** @} */ // end of doxygen group 

