/**
 * \file CBAlgoMergeShortestDistance.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoMergeShortestDistance
 * This algorithm will merge small hits into nearby large clusters
 * You can set the minimum distance, which will be the closest approach
 * as well as the average distance between all hits.
 *
 * @author Corey Adams
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOMERGESHORTESTDISTANCE_H
#define RECOTOOL_CBALGOMERGESHORTESTDISTANCE_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"


namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CBAlgoMergeShortestDistance : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoMergeShortestDistance();
    
    /// Default destructor
    virtual ~CBAlgoMergeShortestDistance(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
                      const ::cluster::ClusterParamsAlg &cluster2);


    float getShortestDist(const ::cluster::ClusterParamsAlg &cluster1,
                          const ::cluster::ClusterParamsAlg &cluster2);

    void SetMaxDistance(float d){_max_distance = d;}

  private:
    
    float _max_distance;
  };
}

#endif
/** @} */ // end of doxygen group 

