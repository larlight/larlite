/**
 * \file CFAlgoProhibitPlane.h
 *
 * \ingroup CMTAlgMatch
 * 
 * \brief Class def header for a class CFAlgoProhibitPlane
 *
 * @author david_NAME
 */

/** \addtogroup CMTAlgMatch

    @{*/
#ifndef CFALGOPROHIBITPLANE_HH
#define CFALGOPROHIBITPLANE_HH

#include "CMTool/CMToolBase/CFloatAlgoBase.h"

namespace cmtool {
  /**
     \class CFAlgoProhibitPlane
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgoProhibitPlane : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgoProhibitPlane();
    
    /// Default destructor
    virtual ~CFAlgoProhibitPlane(){};

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::cluster_params*> &clusters);

    /**
       Optional function: called after each iterative approach if a manager class is
       run with verbosity level <= kPerIteration. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance, called together with manager's Reset()
    virtual void Reset();

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
     */
    //virtual void EventBegin(const std::vector<cluster::ClusterParamsAlg> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    //virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iterative loop.
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlg> &clusters);

    /**
       Optional function: called at the end of each iterative loop.
     */
    //virtual void IterationEnd();

    void setPlaneRemoved(int pl) { _removePlane = pl; }

  private:

    int _removePlane;

  };
}
#endif
/** @} */ // end of doxygen group 

