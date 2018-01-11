/**
 * \file CBAlgoVtxAlign3.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoVtxAlign3
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOVTXALIGN3_H
#define RECOTOOL_CBALGOVTXALIGN3_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"
#include <limits>

namespace cmtool {
  /**
     \class CBAlgoVtxAlign3
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBAlgoVtxAlign3 : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoVtxAlign3();
    
    /// Default destructor
    virtual ~CBAlgoVtxAlign3(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual std::vector<std::vector<size_t> > Merge(const std::vector<::cluster::Cluster>& clus_v);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    void SetMaxAngleDiff  (float a) { _max_angle_diff_merge = a;  }
    void SetMinParOAngle(float a) { _min_parpar_oangle = a; }
    void SetMinNHits(size_t n) { _min_nhits = n; }
    void SetMaxMergeDist(float d) { _max_merge_dist = d; }

  protected:

    float ClusterDistance(const ::cluster::Cluster& c1, const ::cluster::Cluster& c2);

    float _max_angle_diff_merge;
    float _min_parpar_oangle;
    float _max_merge_dist; // fraction of length of larger shower
    size_t _min_nhits;
    

    int _inv_int;
    float _inv_float;
    double _inv_double;
    size_t _inv_size;
    
  };
}

#endif
/** @} */ // end of doxygen group 

