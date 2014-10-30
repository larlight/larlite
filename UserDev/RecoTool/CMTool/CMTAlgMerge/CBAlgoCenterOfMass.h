/**
 * \file CBAlgoCenterOfMass.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoCenterOfMass
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOCENTEROFMASS_H
#define RECOTOOL_CBALGOCENTEROFMASS_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include <math.h>

namespace cmtool {
  /**
     \class CBAlgoCenterOfMass
     Designed mainly for small clusters (< 10 hits):
     Find Cluster's center of mass weighing by Q
     Then see if COM is in polygon/cone of a big cluster
     If so -> merge
  */
  class CBAlgoCenterOfMass: public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoCenterOfMass();
    
    /// Default destructor
    virtual ~CBAlgoCenterOfMass(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging iteration
    virtual void Report();

    /// Function to set Debug mode of output
    void SetDebug(bool on) { _debug = on; }

    /// Function to set Max hits for small clsuters
    void SetMaxHitsSmallClus(size_t n) { _maxHits = n; }

    /// Function to se Min hits for big clusters
    void SetMinHitsBigClus(size_t n) { _minHits = n; }

    /// Function to set Max Distance for COM to be from start-end
    void SetMaxDistance(double d) { _MaxDist = d; }

    /// Use COM in Poly algo to decide merging
    void UseCOMInPoly(bool on) { _COMinPolyAlg = on; }

    /// Use COM in Poly algo to decide merging
    void UseCOMInCone(bool on) { _COMinConeAlg = on; }

    /// Use COM in Poly algo to decide merging
    void UseCOMNearClus(bool on) { _COMNearClus = on; }

    /// Set Length Reach: How for out the cone extends as percent of cluster length
    void SetLengthReach(double frac){ _lengthReach = frac; }

    double ShortestDistanceSquared(double point_x, double point_y, 
				   double start_x, double start_y,
				   double end_x,   double end_y  ) const;
    
    
  protected:

    bool _debug;
    size_t _minHits;
    size_t _maxHits;
    double _MaxDist;
    double _lengthReach; ///How four out - as percent of cluster length - cone will extend from start point
    bool _COMinPolyAlg;
    bool _COMinConeAlg;
    bool _COMNearClus;
  };
}

#endif
/** @} */ // end of doxygen group 

