/**
 * \file CBAlgoPolyShortestDist.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CBAlgoPolyShortestDist
 *
 * @author davidkaleko
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBALGOPOLYSHORTESTDIST_H
#define RECOTOOL_CBALGOPOLYSHORTESTDIST_H

#include <iostream>
#include "CMTool/CMToolBase/CBoolAlgoBase.h"

namespace cmtool {
  /**
     \class CBAlgoPolyShortestDist
     User implementation for CBoolAlgoBase class
     doxygen documentation!
  */
  class CBAlgoPolyShortestDist : public CBoolAlgoBase {
    
  public:
    
    /// Default constructor
    CBAlgoPolyShortestDist();
    
    /// Default destructor
    virtual ~CBAlgoPolyShortestDist(){};

    /**
       Optional function: called at the beginning of 1st iteration. This is called per event.
    */
    virtual void EventBegin(const std::vector<cluster::cluster_params> &clusters);

    /**
       Optional function: called at the end of event ... after the last merging iteration is over.
     */
    //virtual void EventEnd();
 
    /**
       Optional function: called at the beggining of each iteration over all pairs of clusters. 
       This provides all clusters' information in case the algorithm need them. Note this
       is called per iteration which may be more than once per event.
     */
    //virtual void IterationBegin(const std::vector<cluster::cluster_params> &clusters);

    /**
       Optional function: called at the end of each iteration over all pairs of clusters.
     */
    //virtual void IterationEnd();

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

    //both clusters must have > this # of hits to be considered for merging
    void SetMinNumHits(size_t nhits) { _min_hits = nhits; }

    void SetMaxNumHits(int nhits) { _max_hits = nhits; }

    void SetMinDistSquared(double dist) { _dist_sqrd_cut = dist; }

    void SetDebug(bool flag) { _debug = flag; }

  private:


    size_t _min_hits, _max_hits;

    double _dist_sqrd_cut;

    bool _debug;

    double tmp_min_dist;
  };
}
#endif
/** @} */ // end of doxygen group 

