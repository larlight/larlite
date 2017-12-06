/**
 * \file CBoolAlgoBase.hh
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for algorithm classes for CMergeManager
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CBOOLALGOBASE_H
#define RECOTOOL_CBOOLALGOBASE_H

#include "CMAlgoBase.h"

namespace cmtool {

  /**
     \class CBoolAlgoBase
     An abstract base class for merging algorithm. Having this base class helps
     to have a better overall design of various merging for iterative approach.
     The algorithms are run through CMergeManager.
  */
  class CBoolAlgoBase : public CMAlgoBase {
    
  public:
    
    /// Default constructor
    CBoolAlgoBase(){ _merge_till_converge = false; _pair_wise = true; }
    
    /// Default destructor
    virtual ~CBoolAlgoBase(){}

    /**
       Core function: given the CPAN input, return whether a cluster should be
       merged or not.
    */
    virtual bool Bool(const ::cluster::Cluster &cluster1,
                      const ::cluster::Cluster &cluster2)
    { if(cluster1._plane != cluster2._plane) return false;
      else return true;
    }

    /**
       
       Core function: given full event info, return matched index vectors
     */
    virtual std::vector< std::vector<size_t> > Merge(const std::vector<::cluster::Cluster>& clus_v)
    { return std::vector< std::vector<size_t> >{}; }

    /**
       Set MergeTillConverge flag
     */
    void SetMergeTillConverge(bool on) { _merge_till_converge = on;   }
    bool MergeTillConverge()           { return _merge_till_converge; }

    /**
       Use algorithm pair-wise or with full list of event clusters?
     */
    void SetUsePairWise(bool on) { _pair_wise = on;   }
    bool PairWiseMode()          { return _pair_wise; }

  private:

    bool _merge_till_converge;

    bool _pair_wise;

  };

}

#endif
/** @} */ // end of doxygen group 

