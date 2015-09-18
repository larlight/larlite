/**
 * \file CBAlgoCenterOfMassSmall.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoCenterOfMassSmall
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOCENTEROFMASSSMALL_H
#define RECOTOOL_CBALGOCENTEROFMASSSMALL_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include <math.h>

namespace cmtool {
  /**
     \class CBAlgoCenterOfMassSmall
     Designed mainly for small clusters (< 10 hits):
     Find Cluster's center of mass weighing by Q
     Then see if COM is in polygon/cone of a big cluster
     If so -> merge
  */
  class CBAlgoCenterOfMassSmall: public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoCenterOfMassSmall();
    
    /// Default destructor
    virtual ~CBAlgoCenterOfMassSmall(){};
 
    /**
       Core function: given the cluster_params input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging iteration
    virtual void Report();

    /// Function to set Debug mode of output
    void SetDebug(bool on) { _debug = on; }

    /// Function to set Max hits for small clsuters
    void SetMaxHitsSmallClus(size_t n) { _maxHits = n; }

    /// Function to set Max Distance for COM to be from start-end
    void SetMaxDistance(double d) { _MaxDist = d; }

    /// Function to set Max Distance between COMs
    void SetMaxCOMDistance(double d) { _MaxCOMDistSquared = d; }

    /// Use COM in Poly algo to decide merging
    void UseCOMInPoly(bool on) { _COMinPolyAlg = on; }

    /// Use COM in Poly algo to decide merging
    void UseCOMClose(bool on) { _COMsClose = on; }

    /// Use COM in Poly algo to decide merging
    void UseCOMNearClus(bool on) { _COMNearClus = on; }

    /// Set Length Reach: How for out the cone extends as percent of cluster length
    void SetLengthReach(double frac){ _lengthReach = frac; }

    double ShortestDistanceSquared(double point_x, double point_y, 
				   double start_x, double start_y,
				   double end_x,   double end_y  ) const;
    
    
  protected:

    bool _debug;
    size_t _maxHits;
    double _MaxDist; //Max distance between COM and start-end line of cluster
    double _MaxCOMDistSquared; //Max distance^2 between COMs in cm^2
    double _lengthReach; ///How four out - as percent of cluster length - cone will extend from start point
    bool _COMinPolyAlg;
    bool _COMsClose;
    bool _COMNearClus;
  };
}

#endif
/** @} */ // end of doxygen group 

