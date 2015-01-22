/**
 * \file MCMatchAlg.h
 *
 * \ingroup MCComp
 * 
 * \brief Class def header for a class MCMatchAlg
 *
 * @author kazuhiro
 */

/** \addtogroup MCComp

    @{*/
#ifndef RECOTOOL_MCMATCHALG_H
#define RECOTOOL_MCMATCHALG_H

#include <iostream>
#include "DataFormat/simch.h"
#include "DataFormat/hit.h"
#include "LArUtil/Geometry.h"
//#include "Geometry/Geometry.h"
#include "MCBTAlg.h"

namespace btutil {
  /**
     \class MCMatchAlg
     User defined class MCMatchAlg ... these comments are used to generate
     doxygen documentation!
  */
  class MCMatchAlg {
    
  public:
    
    /// Default constructor
    MCMatchAlg();
    
    /// Default destructor
    virtual ~MCMatchAlg(){};

    
    /// Constructs needed information for Reco=>MC matching
    bool BuildMap(const std::vector< unsigned int>     &g4_trackid_v,
		  const std::vector< ::larlite::simch> &simch_v,
		  const std::vector< ::larlite::hit>   &hit_v,
		  const std::vector<std::vector<unsigned int> > &cluster_hit_association
		  );
      
    /**
       For a specified MC object (via index in MCShower/MCTrack), returns best-matched clusters 
       represented by an index number of clusters. The return vector length is same as
       # of planes, and "-1" if there is no cluster found on the corresponding plane.
    */
    const std::vector<int>& BestClusters(const size_t mcshower_index) const;

    /**
       For a specified cluster, compute the correctness for the specified
       MC object
     */
    double ClusterCorrectness(const size_t cluster_index,
			      const size_t mcshower_index) const;

    /**
       Similar to ClusterCorrectness() function except this one computes the correctness for multiple
       clusters. It is meant to compute the correctness for a cluster combination that is matched 
       across plane to represent one 3D reco object. The return is a pair of best-matched MC object
       and correctness.
     */
    std::pair<size_t,double> ShowerCorrectness(const std::vector<unsigned int> cluster_indices) const;


    ///For a specified cluster, compute cluster efficiency and purity in terms of specified MC object
    std::pair<double,double>  ClusterEP(const size_t cluster_index,
					const size_t mcshower_index) const;

    /**
       For a specified MC object, compute the best-matched cluster's efficiency and purity from 
       the specified plane ID. Internally this function calls BestClusters() to find the best 
       cluster on each plane, then calls ClusterEP() to retrieve the efficiency and purity.
     */
    std::pair<double,double> BestClusterEP(const size_t mcshower_index,
					   const size_t plane_id) const;

    /// BTAlgo getter
    const MCBTAlg& BTAlg() const { return fBTAlgo; }

  protected:

    /// MCBTAlg instance 
    MCBTAlg fBTAlgo;

    std::vector<size_t> _view_to_plane;

    std::vector<std::vector<double> > _summed_mcq;
    std::vector<std::vector<double> > _cluster_mcq_v;

    std::vector<unsigned char> _cluster_plane_id;
    std::vector<std::vector<int> > _bmatch_id;

  };
}
#endif
/** @} */ // end of doxygen group 

