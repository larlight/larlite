/**
 * \file CBAlgoVtxAlign.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoVtxAlign
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOVTXALIGN_H
#define RECOTOOL_CBALGOVTXALIGN_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoVtxAlign
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBAlgoVtxAlign : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoVtxAlign();
    
    /// Default destructor
    virtual ~CBAlgoVtxAlign(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual std::vector<std::vector<size_t> > Merge(const std::vector<::cluster::Cluster>& clus_v);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    void SetMaxAngleDiff  (float a) { _max_angle_diff_merge = a;  }
    void SetMinGammaOAngle(float a) { _min_gammagamma_oangle = a; }
    void SetMinNHits(size_t n) { _min_nhits = n; }
    void SetMaxMergeDist(float d) { _max_merge_dist = d; }

  protected:

    float ClusterDistance(const ::cluster::Cluster& c1, const ::cluster::Cluster& c2);

    float _max_angle_diff_merge;
    float _min_gammagamma_oangle;
    float _max_merge_dist; // fraction of length of larger shower
    size_t _min_nhits;

  };
}

#endif
/** @} */ // end of doxygen group 

