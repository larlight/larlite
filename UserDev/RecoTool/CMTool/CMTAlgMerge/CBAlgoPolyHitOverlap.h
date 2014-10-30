/**
 * \file CMalgoPolyHitOverlap.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoPolyHitOverlap
 *
 * @author David Caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPOLYHITOVERLAP_H
#define RECOTOOL_CBALGOPOLYHITOVERLAP_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"

namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if one is completely inside the other
  */
  class CBAlgoPolyHitOverlap : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoPolyHitOverlap();
    
    /// Default destructor
    virtual ~CBAlgoPolyHitOverlap(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /// Method to re-configure the instance
    void reconfigure();

  };
}

#endif
/** @} */ // end of doxygen group 

