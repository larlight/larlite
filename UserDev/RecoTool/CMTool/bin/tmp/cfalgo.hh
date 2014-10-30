/**
 * \file CFAlgo_Class_Name.hh
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class CFAlgo_Class_Name
 *
 * @author USER_NAME
 */

/** \addtogroup Working_Package

    @{*/
#ifndef CFALGO_CLASS_NAME_HH
#define CFALGO_CLASS_NAME_HH

#include "CFloatAlgoBase.hh"

namespace cmtool {
  /**
     \class CFAlgo_Class_Name
     User implementation for CFloatAlgoBase class
     doxygen documentation!
  */
  class CFAlgo_Class_Name : public CFloatAlgoBase {
    
  public:
    
    /// Default constructor
    CFAlgo_Class_Name();
    
    /// Default destructor
    virtual ~CFAlgo_Class_Name(){};

    //
    // Author should be aware of 3 functions at least: Float, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given a set of CPANs, return a float which indicates 
       the compatibility the cluster combination.
    */
    virtual float Float(const std::vector<const cluster::ClusterParamsAlg*> &clusters);

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

  };
}
#endif
/** @} */ // end of doxygen group 

