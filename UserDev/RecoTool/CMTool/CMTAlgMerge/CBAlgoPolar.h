/**
 * \file CBAlgoPolar.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoPolar
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPOLAR_H
#define RECOTOOL_CBALGOPOLAR_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoPolar
     An abstract fake class for merging algorithm. Having this fake class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBAlgoPolar : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoPolar();
    
    /// Default destructor
    virtual ~CBAlgoPolar(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::Cluster &cluster1,
                      const ::cluster::Cluster &cluster2);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    void SetBufferAngle(const float& b) { _buffer = b; }

  protected:

    float _buffer;
    
    float ClusterDistance(const ::cluster::Cluster& c1, const ::cluster::Cluster& c2);
    
  };
}

#endif
/** @} */ // end of doxygen group 

