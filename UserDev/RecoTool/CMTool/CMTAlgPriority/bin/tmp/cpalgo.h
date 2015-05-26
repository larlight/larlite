/**
 * \file CPAlgo_Class_Name.h
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class CPAlgo_Class_Name
 *
 * @author USER_NAME
 */

/** \addtogroup Working_Package

    @{*/
#ifndef CPALGO_CLASS_NAME_H
#define CPALGO_CLASS_NAME_H

#include "CPriorityAlgoBase.h"

namespace cmtool {
  /**
     \class CPAlgo_Class_Name
     User implementation for CPriorityAlgoBase class
     doxygen documentation!
  */
  class CPAlgo_Class_Name : public CPriorityAlgoBase {
    
  public:
    
    /// Default constructor
    CPAlgo_Class_Name();
    
    /// Default destructor
    virtual ~CPAlgo_Class_Name(){};

    //
    // Author should be aware of 3 functions at least: Priority, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given the CPAN input, return a float which indicates 
       the user-defined priority for analysis.
    */
    virtual float Priority(const ::cluster::ClusterParamsAlg &cluster);

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

