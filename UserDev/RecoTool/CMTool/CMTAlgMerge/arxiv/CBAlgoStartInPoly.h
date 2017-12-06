/**
 * \file CBAlgoStartInPoly.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoStartInPoly
 *
 * @author David Caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOSTARTINPOLY_H
#define RECOTOOL_CBALGOSTARTINPOLY_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"


namespace cmtool {
  /**
     \class CMalgoStartInPoly
     If start point of one cluster inside other's polygon -> merge
  */
  class CBAlgoStartInPoly : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoStartInPoly();
    
    /// Default destructor
    virtual ~CBAlgoStartInPoly(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */

    /// Method to set cut value on minimum number of hits considered
    void SetMinHitsCut(size_t n) { _MinHits = n; }

    void SetDebug(bool debug) { _debug = debug; }

    /// Merging Algorithm is Here
    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);


    /// Method to re-configure the instance
    void reconfigure();

  protected:

    double _wire_2_cm, _time_2_cm; /// Conversion factors ogtten from GeometryUtilities
    size_t _MinHits; /// Minimum number of hits for cluster whose start point is being considered. We want it to be a good start point...
    bool _debug;
  };
}

#endif
/** @} */ // end of doxygen group 

