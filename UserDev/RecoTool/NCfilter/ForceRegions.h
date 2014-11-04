/**
 * \file ForceRegions.h
 *
 * \ingroup NCfilter
 * 
 * \brief Class def header for a class ForceRegions
 *
 * @author ryan
 */

/** \addtogroup NCfilter

    @{*/

#ifndef RECOTOOL_FORCEREGIONS_H
#define RECOTOOL_FORCEREGIONS_H

#include <iostream>
#include <vector>
#include "LArUtil/LArUtilBase.h"
#include "LArUtil/PxUtils.h"
#include "ClusterRecoUtil/ClusterParamsAlg.h"

namespace cluster {
  /**
     \class ForceRegions
     Describe me!
   */
  class ForceRegions{

  public:

    /// Default constructor
    ForceRegions(){};

    /// Default destructor
    virtual ~ForceRegions(){};

    /**
       Clustering function. Returns a vector of clusters.
       Each cluster is represented as a group of hit via vector of hit indexes.
    */
	// what ever I want it to return 
  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoRegions(const std::vector<larlite::hit>& hits, std::vector<std::pair<double,double>> APP);

  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoFlipRegions(const std::vector<larlite::hit>& hits, std::vector<std::pair<double,double>> APP);

  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceTwoRotateRegions(const std::vector<larlite::hit>& hits, std::vector<std::pair<double,double>> APP, double angle);

  };

}
#endif
/** @} */ // end of doxygen group 
