/**
 * \file CBAlgoFake.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoFake
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOFAKE_H
#define RECOTOOL_CBALGOFAKE_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoFake
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBAlgoFake : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoFake();
    
    /// Default destructor
    virtual ~CBAlgoFake(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

  protected:

    bool _flip;
    int _ctr;
  };
}

#endif
/** @} */ // end of doxygen group 

