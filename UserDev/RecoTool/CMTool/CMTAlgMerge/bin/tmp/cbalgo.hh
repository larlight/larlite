/**
 * \file CBAlgo_Class_Name.hh
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class CBAlgo_Class_Name
 *
 * @author USER_NAME
 */

/** \addtogroup Working_Package

    @{*/
#ifndef CBALGO_CLASS_NAME_HH
#define CBALGO_CLASS_NAME_HH

#include "CBoolAlgoBase.hh"

namespace cmtool {
  /**
     \class CBAlgo_Class_Name
     User implementation for CBoolAlgoBase class
     doxygen documentation!
  */
  class CBAlgo_Class_Name : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgo_Class_Name();
    
    /// Default destructor
    virtual ~CBAlgo_Class_Name(){};

    //
    // Author should be aware of 3 functions at least: Bool, Report, 
    // and Reset. More possibly-useful functions can be found in the later 
    // part but commented out. All of these functions are virtual and defined
    // in the base class.

    /**
       Core function: given two CPAN inputs, return a boolean which indicates 
       whether two clusters are compatible or not
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

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

