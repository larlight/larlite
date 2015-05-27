/**
 * \file CBAlgoMergeInline.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoMergeInline
 * This algorithm finds the closest distance between each hit and
 * all the hits in the other cluster.  If the average is below 
 * a criteria, it returns true for merging.
 * Meant to pull in tracks that are basically on top of each other
 *
 * @author Corey Adams
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOMERGEINLINE_H
#define RECOTOOL_CBALGOMERGEINLINE_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"


namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CBAlgoMergeInline : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoMergeInline();
    
    /// Default destructor
    virtual ~CBAlgoMergeInline(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
                      const ::cluster::ClusterParamsAlg &cluster2);



    float getAverageDist(const ::cluster::ClusterParamsAlg &cluster1,
                         const ::cluster::ClusterParamsAlg &cluster2);

    float getBinnedDist (const ::cluster::ClusterParamsAlg &cluster1,
                         const ::cluster::ClusterParamsAlg &cluster2);

    void SetMaxAverageMinDistance(float d){_max_average_min_distance = d;}
    void SetUseAllHits(bool b){_use_all_hits = b;}
    void SetHitFraction(float d){_hit_fraction = d;}

  private:
    
    float _max_average_min_distance;
    float _hit_fraction;
    bool _use_all_hits;
  };
}

#endif
/** @} */ // end of doxygen group 

