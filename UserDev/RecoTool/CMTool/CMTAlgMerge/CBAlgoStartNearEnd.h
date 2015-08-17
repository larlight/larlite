/**
 * \file CBAlgoStartNearEnd.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoStartNearEnd
 *
 * @author david caratelli
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOSTARTNEAREND_H
#define RECOTOOL_CBALGOSTARTNEAREND_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoStartNearEnd
     Merge if start point of one is near end point of another
     and require angle compatibility
  */
  class CBAlgoStartNearEnd : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoStartNearEnd();
    
    /// Default destructor
    virtual ~CBAlgoStartNearEnd(){};
 
    /**
       Core function: given the ClusterParamsAlg input, return whether a cluster should be
       merged or not.
    */

    void SetMaxStartEndSeparation(double d) { _separation=d; }

    void SetMaxAngle(double a) { _maxopeningangle=a; }

    void SetMinHits(size_t n) { _MinHits=n; }

    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);


    /// Function to reset the algorithm instance ... maybe implemented via child class
    virtual void Reset(){}

    /// Function to report what's going on per merging
    virtual void Report();

  protected:

    double _maxopeningangle;
    double _separation;
    size_t _MinHits;

  };
}

#endif
/** @} */ // end of doxygen group 

