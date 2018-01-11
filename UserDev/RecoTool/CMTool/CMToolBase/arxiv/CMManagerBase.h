/**
 * \file CMManagerBase.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class CMManagerBase
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_CMMANAGERBASE_H
#define RECOTOOL_CMMANAGERBASE_H

#include <iostream>
#include <set>
#include <map>

#include "CPriorityAlgoBase.h"
#include "TStopwatch.h"
#include "ClusterRecoUtil/Alg/DefaultParamsAlg.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"

namespace cmtool {

  /**
     \class CMManagerBase
     A class that instantiates merging algorithm(s) and run.
     The book-keeping of merged cluster sets are done by CMergeBookKeeper.
  */
  class CMManagerBase{

  public:

    /// Enum to specify message output level
    enum CMMSGLevel_t {
      /// Extremely verbose (cout per individual algorithm execution)
      kPerMerging,
      /// Somewhat verbose (cout per merging iteration)
      kPerIteration,
      /// Bit verbose (cout per event)
      kPerEvent,
      /// Normal
      kNone
    };
    
  public:
    
    /// Default constructor
    CMManagerBase();
    
    /// Default destructor
    virtual ~CMManagerBase(){}

    /// Method to enable debug mode (lots of couts)
    void DebugMode(CMMSGLevel_t level) {_debug_mode=level;}

    /// Method to enable timing profile cout
    void ReportTimings(bool time_report=true) { _time_report = time_report; }

    /// Method to reset itself
    void Reset();

    /// Setter to add an algorithm for priority determination
    void AddPriorityAlgo(CPriorityAlgoBase* algo) { _priority_algo = algo; }

    /// Switch to continue merging till converges
    void MergeTillConverge(bool doit=true) {_merge_till_converge = doit;}

    /// A simple method to add a cluster
    void SetClusters(const std::vector<std::vector<larutil::Hit2D> > &clusters);

    /// A simple method to add a cluster
    void SetClusters(const std::vector<::cluster::cluster_params> &clusters);

#ifndef __CINT__
    /// A simple method to add a cluster
    void SetClusters(std::vector<::cluster::cluster_params>&& clusters);
#endif
    /// A getter for input clusters
    const std::vector<::cluster::cluster_params>&  GetInputClusters() const 
    { return _in_clusters; }

    /// A setter for minimum # of hits ... passed onto cluster_params
    void SetMinNHits(unsigned int n) { _min_nhits = n; }

    /// A method to execute the main action, to be called per event
    void Process();

    /// A setter for an analysis output file
    void SetAnaFile(TFile* fout) { _fout = fout; }

  protected:

    /// Function to compute priority
    void ComputePriority(const std::vector<::cluster::cluster_params> & clusters);

    /// FMWK function called @ beginning of Process()
    virtual void EventBegin(){}

    /// FMWK function called @ beginning of iterative loop inside Process()
    virtual void IterationBegin(){}

    /// FMWK function called @ iterative loop inside Process()
    virtual bool IterationProcess()=0;

    /// FMWK function called @ end of iterative loop inside Process()
    virtual void IterationEnd(){}

    /// FMWK function called @ end of Process()
    virtual void EventEnd(){}

  protected:

    /// Timing verbosity flag
    bool _time_report;
    
    /// Minimum number of hits: the limit set for cluster_params
    unsigned int _min_nhits;

    /// Debug mode switch
    CMMSGLevel_t _debug_mode;

    /// Input clusters
    std::vector<::cluster::cluster_params> _in_clusters;

    /// Cluster Params Filler:
    ::cluster::DefaultParamsAlg _params_alg;

    /// Priority algorithm
    ::cmtool::CPriorityAlgoBase* _priority_algo;

    /// Output analysis plot TFile
    TFile* _fout;

    /// Priority record 
    std::multimap<float,size_t> _priority;

    /// Iteration loop switch
    bool _merge_till_converge;

    /// A holder for # of unique planes in the clusters, computed in ComputePriority() function
    std::set<UChar_t> _planes;

  };
}

#endif
/** @} */ // end of doxygen group 

