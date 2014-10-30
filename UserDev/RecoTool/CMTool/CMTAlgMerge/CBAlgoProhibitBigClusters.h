/**
 * \file CBAlgoProhibitBigClusters.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoProhibitBigClusters
 *
 * @author davidkaleko
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPROHIBITBIGCLUSTERS_H
#define RECOTOOL_CBALGOPROHIBITBIGCLUSTERS_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoProhibitBigClusters
     User implementation for CBoolAlgoBase class
     doxygen documentation!
  */
  class CBAlgoProhibitBigClusters : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoProhibitBigClusters();
    
    /// Default destructor
    virtual ~CBAlgoProhibitBigClusters(){};

    //
    // Author should be aware of 3 functions at least: Bool, Report, and Reset.
    // More possibly-useful functions can be later part but commented out.
    // All of these functions are virtual and defined in the base class.
    //

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::ClusterParamsAlg &cluster1,
		      const ::cluster::ClusterParamsAlg &cluster2);

    /**
       Optional function: called after each Merge() function call by CMergeManager IFF
       CMergeManager is run with verbosity level kPerMerging. Maybe useful for debugging.
    */
    virtual void Report();
    
    /// Function to reset the algorithm instance ... maybe implemented via child class
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
       Optional function: called at the beggining of each iteration over all pairs of clusters. 
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::ClusterParamsAlg> &clusters);

    /**
       Optional function: called at the end of each iteration over all pairs of clusters.
     */
    //virtual void IterationEnd();

    void SetMinHits(size_t nhits) { _min_hits = nhits; }

  protected:

    size_t _min_hits;

  };
}
#endif
/** @} */ // end of doxygen group 

