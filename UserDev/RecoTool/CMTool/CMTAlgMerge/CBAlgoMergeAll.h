/**
 * \file CBAlgoMergeAll.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoMergeAll
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOMERGEALL_H
#define RECOTOOL_CBALGOMERGEALL_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoMergeAll
     Merges all clusters: maybe useful to test how well a cluster-separating
     algorithm has performed
  */
  class CBAlgoMergeAll: public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoMergeAll();
    
    /// Default destructor
    virtual ~CBAlgoMergeAll(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    ///Set hits threshold for 2 clusters to be merged
    void SetHits( int hits ) { _hits = hits ; }

  protected:


  int _hits ;

  };
}

#endif
/** @} */ // end of doxygen group 

