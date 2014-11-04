/*
 * @author Ryan
 */


#ifndef RECOTOOL_DIVREG_H
#define RECOTOOL_DIVREG_H

#include <iostream>
#include <vector>
#include "LArUtil/PxUtils.h"
#include "LArUtil/LArUtilBase.h"
#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"
#include "ClusterRecoUtil/ClusterParamsAlg.h"

namespace cluster {
  class DivReg{

  public:

    /// Default constructor
    DivReg(){};

    /// Default destructor
    virtual ~DivReg(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
	// what ever I want it to return 
   // std::vector<std::vector<unsigned int> > Cluster(const std::vector<std::vector<larutil::PxHit>>& hits) const;
  std::vector<std::pair<double,double>> SplitLine(const std::vector<std::vector<larutil::PxHit>>& hits) const;
  std::vector<std::pair<double,double>> SplitLineC(const std::vector<larlite::cluster>& clusters,
						   const std::vector<std::vector<unsigned int> >& cluster_hit_ass,
						   const std::vector<larlite::hit>& hits) const;
  std::vector<std::pair<double,double>> FlipLineC(const std::vector<larlite::cluster>& clusters,
						  const std::vector<std::vector<unsigned int> >& cluster_hit_ass,
						  const std::vector<larlite::hit>& hits) const;
  std::vector<std::pair<double,double>> RotateLineC(const std::vector<larlite::cluster>& clusters,
						    const std::vector<std::vector<unsigned int> >& cluster_hit_ass,
						    const std::vector<larlite::hit>& hits) const;

  };
}
#endif
