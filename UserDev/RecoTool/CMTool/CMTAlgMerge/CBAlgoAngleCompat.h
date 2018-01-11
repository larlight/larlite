/**
 * \file CBAlgoAngleCompat.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoAngleCompat
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOANGLECOMPAT_H
#define RECOTOOL_CBALGOANGLECOMPAT_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoAngleCompat
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBAlgoAngleCompat : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoAngleCompat();
    
    /// Default destructor
    virtual ~CBAlgoAngleCompat(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::Cluster &cluster1,
                      const ::cluster::Cluster &cluster2);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    void SetMinLargeNHits(size_t n) { _min_size = n; }
    void SetMaxAngleDiff(float a) { _max_angle_diff = a; }

  protected:

    size_t _min_size;
    float  _max_angle_diff;

    bool _flip;
    int _ctr;
  };
}

#endif
/** @} */ // end of doxygen group 

