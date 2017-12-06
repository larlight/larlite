/**
 * \file CBAlgoMergeTinyWithBig.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoMergeTinyWithBig
 *
 * @author davidkaleko
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOMERGETINYWITHBIG_H
#define RECOTOOL_CBALGOMERGETINYWITHBIG_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoMergeTinyWithBig
     User implementation for CBoolAlgoBase class
     doxygen documentation!
     
     This algo looks for tiny clusters and looks to merge them with
     big clusters. It uses code from PolyShortestDist and says if
     the small cluster is close(ish) to the big one, merge it.
     
  */
  class CBAlgoMergeTinyWithBig : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoMergeTinyWithBig();
    
    /// Default destructor
    virtual ~CBAlgoMergeTinyWithBig(){};

    //
    // Author should be aware of 3 functions at least: Bool, Report, and Reset.
    // More possibly-useful functions can be later part but commented out.
    // All of these functions are virtual and defined in the base class.
    //

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::cluster_params &cluster1,
                      const ::cluster::cluster_params &cluster2);

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

    //both clusters must have > this # of hits to be considered for merging
    void SetMinHitsBig(size_t nhits) { _min_hits_big = nhits; }

    void SetMaxHitsBig(size_t nhits) { _max_hits_big = nhits; }

    void SetMinHitsSmall(size_t nhits) { _min_hits_small = nhits; }

    void SetMaxHitsSmall(size_t nhits) { _max_hits_small = nhits; }

    void SetMinDistSquared(double dist) { _dist_sqrd_cut = dist; }

    void SetDebug(bool flag) { _debug = flag; }

  private:

    size_t _min_hits_small, _max_hits_small, _min_hits_big, _max_hits_big;

    double _dist_sqrd_cut;

    bool _debug;

  };
}
#endif
/** @} */ // end of doxygen group 

