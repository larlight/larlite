/**
 * \file CBAlgoProhibitStartToStart.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoProhibitStartToStart
 *
 * @author Corey Adams
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPROHIBITSTARTTOSTART_H
#define RECOTOOL_CBALGOPROHIBITSTARTTOSTART_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"


namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CBAlgoProhibitStartToStart : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoProhibitStartToStart();
    
    /// Default destructor
    virtual ~CBAlgoProhibitStartToStart(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
                      const ::cluster::ClusterParamsAlg &cluster2);

    void SetMinSeparation(float n){_min_separation = n;}
    void SetMinHits(int n){_min_hits = n;}

    float getDistance(larutil::PxPoint p1, larutil::PxPoint p2);
    void SetDebug(bool b){debug = b;}

  private:
    
    bool debug;
    float _min_separation;
    int _min_hits;
  };
}

#endif
/** @} */ // end of doxygen group 

