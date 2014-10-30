/**
 * \file CBAlgoStartInCone.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoStartInCone
 *
 * @author david
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOSTARTINCONE_H
#define RECOTOOL_CBALGOSTARTINCONE_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"
#include <math.h>

namespace cmtool {
  
  /**
     \class CBAlgoStartInCone
     If start point of Cluster B in Cone region of Cluster A then merge
  */
  class CBAlgoStartInCone : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoStartInCone();
    
    /// Default destructor
    virtual ~CBAlgoStartInCone(){};
  
    /// Merging Algorithm is Here
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);
    
    /// Method to re-configure the instance
    void reconfigure();

    /// Set Minimum number of hits for cone-cluster
    void SetMinHits(size_t n){ _NhitsMin = n; }

    /// Set Minimum number of hits for cone-cluster
    void SetMinLen(double l){ _lenMin = l; }

    /// Set Verbosity of messages
    void SetVerbose(bool verbosity){ _verbose = verbosity; }

    /// Set Debug for messages
    void SetDebug(bool debug){ _debug = debug; }

    /// Set Angle Compatibility betweeen the clusters
    void SetAngleCompat(double deg){ _angleCompat = deg; }

    /// Set Length Reach: How for out the cone extends as percent of cluster length
    void SetLengthReach(double frac){ _lengthReach = frac; }

  protected:

    double _wire_2_cm, _time_2_cm; /// Conversion factors ogtten from GeometryUtilities

    size_t _NhitsMin;     /// Larger cluster which determines cone must have this many hits
    double _lenMin;    /// Larger cluster which determines cone must be at least this long
    bool _verbose;
    bool _debug;
    double _angleCompat; /// Two clusters must have direction within this value of each other
    double _lengthReach; ///How four out - as percent of cluster length - cone will extend from start point
    
  };

}  
#endif
/** @} */ // end of doxygen group 

