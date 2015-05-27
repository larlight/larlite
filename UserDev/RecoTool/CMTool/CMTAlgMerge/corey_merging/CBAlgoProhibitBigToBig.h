/**
 * \file CBAlgoProhibitBigToBig.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoProhibitBigToBig
 *
 * @author Corey Adams
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPROHIBITBIGTOBIG_H
#define RECOTOOL_CBALGOPROHIBITBIGTOBIG_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include "LArUtil/GeometryUtilities.h"


namespace cmtool {
  /**
     \class CMalgoPolyContain
     Merge Polygons if the two overlap even partially
  */
  class CBAlgoProhibitBigToBig : public CBoolAlgoBase{
    
  public:
    
    /// Default constructor
    CBAlgoProhibitBigToBig();
    
    /// Default destructor
    virtual ~CBAlgoProhibitBigToBig(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
                      const ::cluster::ClusterParamsAlg &cluster2);

    void SetMaxHits(unsigned int n){_max_hits = n;}

  private:
    
    float _max_hits;

  };
}

#endif
/** @} */ // end of doxygen group 

