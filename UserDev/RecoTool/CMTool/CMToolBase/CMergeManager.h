/**
 * \file CMergeManager.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CMergeManager
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CMERGEMANAGER_H
#define RECOTOOL_CMERGEMANAGER_H

#include <iostream>

#include "CMManagerBase.h"
#include "CMergeBookKeeper.h"
#include "CBoolAlgoBase.h"

namespace cmtool {

  /**
     \class CMergeManager
     A class that instantiates merging algorithm(s) and run.
     The book-keeping of merged cluster sets are done by CMergeBookKeeper.
  */
  class CMergeManager : public CMManagerBase {

  public:
    
    /// Default constructor
    CMergeManager();
    
    /// Default destructor
    virtual ~CMergeManager(){}

    /// Method to reset itself
    virtual void Reset();

    /// A simple method to add an algorithm for merging
    void AddMergeAlgo(CBoolAlgoBase* algo) {_merge_algo = algo;}

    /// A simple method to add an algorithm for separation
    void AddSeparateAlgo(CBoolAlgoBase* algo) {_separate_algo = algo;}

    /// A method to obtain output clusters
    const std::vector<::cluster::cluster_params>& GetClusters() const { return _out_clusters; }

    /// A method to give up clusters
#ifndef __CINT__
    void PassOutputClusters(CMergeManager& rhs)
    {
      rhs.SetClusters(std::move(_out_clusters));
    }
#endif

    /// A method to obtain book keeper
    const CMergeBookKeeper& GetBookKeeper() const { return _book_keeper; }

  protected:
    
    //
    // FMWK functions override
    //

    /// FMWK function called @ beginning of Process()
    virtual void EventBegin();

    /// FMWK function called @ beginning of iterative loop inside Process()
    virtual void IterationBegin();

    /// FMWK function called @ iterative loop inside Process()
    virtual bool IterationProcess();

    /// FMWK function called @ end of iterative loop inside Process()
    virtual void IterationEnd();
    
    /// FMWK function called @ end of Process()
    virtual void EventEnd();

  protected:

    void RunMerge(const std::vector<::cluster::cluster_params > &in_clusters,
		  CMergeBookKeeper &book_keeper) const;

    void RunMerge(const std::vector<::cluster::cluster_params > &in_clusters,
		  const std::vector<bool> &merge_flag,
		  CMergeBookKeeper &book_keeper) const;

    void RunSeparate(const std::vector<::cluster::cluster_params > &in_clusters,
		     CMergeBookKeeper &book_keeper) const;

  protected:

    /// Output clusters
    std::vector<cluster::cluster_params> _out_clusters;

    /// Book keeper instance
    CMergeBookKeeper _book_keeper;

    /// Merging algorithm
    ::cmtool::CBoolAlgoBase* _merge_algo;

    /// Separation algorithm
    ::cmtool::CBoolAlgoBase* _separate_algo;

    size_t _iter_ctr;

    std::vector<CMergeBookKeeper> _book_keeper_v;

    std::vector<std::vector<unsigned short> > _tmp_merged_indexes;

    std::vector<::cluster::cluster_params> _tmp_merged_clusters;

  };
}

#endif
/** @} */ // end of doxygen group 

