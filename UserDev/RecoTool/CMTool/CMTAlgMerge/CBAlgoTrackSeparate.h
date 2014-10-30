/**
 * \file CBAlgoTrackSeparate.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoTrackSeparate
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOTRACKSEPARATE_H
#define RECOTOOL_CBALGOTRACKSEPARATE_H

#include <iostream>
#include "math.h"
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoTrackSeparate
  */
  class CBAlgoTrackSeparate : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoTrackSeparate();
    
    /// Default destructor
    virtual ~CBAlgoTrackSeparate(){};

    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    void SetVerbose(bool on) { _verbose = on; }

    void SetDebug(bool on) { _debug = on; }

    void SetMinNumHits(size_t n) { _MinNumHits = n; }

    void SetMinAngleDiff(float anglesep) { _MinAngleDiff = anglesep; }
    
    void SetMaxOpeningAngle(float maxangle) { _MaxOpeningAngle = maxangle; }

    void SetMinLength(float minlength) { _MinLength = minlength; }

    void SetMinPolyHitDensity(float mindensity) { _MinDensity = mindensity; }

    void SetMaxWidth(float maxwidth) { _MaxWidth = maxwidth; }

    void SetUseEP(bool flag) { _use_EP = flag; }

    void SetEPCutoff(float epcut) { _ep_cut = epcut; }

    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging
    virtual void Report();

  protected:

    bool _verbose;
    bool _debug;
    bool _use_EP;
    float _ep_cut;
    size_t _MinNumHits;
    float _MinAngleDiff;
    float _MaxOpeningAngle;
    float _MinLength;
    float _MinDensity;
    float _MaxWidth;
    double _wire_2_cm;
    double _time_2_cm;
  };
}

#endif
/** @} */ // end of doxygen group 

