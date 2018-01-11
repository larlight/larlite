/**
 * \file CBAlgoMergeAll.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoOutOfConeSeparate
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOOUTOFCONESEPARATE_H
#define RECOTOOL_CBALGOOUTOFCONESEPARATE_H

#include <math.h>
#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoAngleSeparate
     Track Prohibit algorithm: if the angle between the direction of a cluster (end-start) and the line connecting the cluster's start point and the start point of t a second cluster is too large, then probihit merging between the two clusters. The first cluster needs to be a "good" and "large" cluster
     algorithm has performed
  */
  class CBAlgoOutOfConeSeparate: public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoOutOfConeSeparate();
    
    /// Default destructor
    virtual ~CBAlgoOutOfConeSeparate(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);


    /// Set Debug Mode on or off
    void SetDebug(bool on) { _debug = on; }

    /// Set Max Angle Separation for separation
    void SetMaxAngleSep(float angle) { _MaxAngle = angle; }

    /// Set Max Angle Separation for separation for far away clusters
    void SetMaxAngleFar(float angle) { _MaxAngleFar = angle; }

    /// Set Distance at which cone-acceptance angle starts falling off as 1/distance. Value should be distance^2 in cm^2
    void SetStartAngleFalloff(float d) { _FallOff = d; }

    /// Set Minimum length for "big" cluster
    void SetMinLength(float len) { _MinLen = len; }

    /// SetMinimum number of hits for small cluster
    void SetMinHits(size_t n) { _minHits = n; }

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging
    virtual void Report(){}

  protected:

    bool _debug;
    float _MaxAngle;
    float _MaxAngleFar;
    float _MinLen;
    float _FallOff;
    size_t _minHits;

  };
}

#endif
/** @} */ // end of doxygen group 

