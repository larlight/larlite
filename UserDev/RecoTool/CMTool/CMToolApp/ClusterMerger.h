/**
 * \file ClusterMerger.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class ClusterMerger
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_CLUSTERMERGER_H
#define RECOTOOL_CLUSTERMERGER_H

#include "Analysis/ana_base.h"
#include "CMTool/CMToolBase/CMergeManager.h"
#include "ClusterRecoUtil/CRUHelper.h"

namespace larlite {
  /**
     \class ClusterMerger
     User custom analysis class made by kazuhiro
   */
  class ClusterMerger : public ana_base{
  
  public:

    /// Default constructor
    ClusterMerger(){ _name="ClusterMerger"; _fout=0; _cluster_producer="fuzzycluster"; _write_output=false;}

    /// Default destructor
    virtual ~ClusterMerger(){}

    /** IMPLEMENT in ClusterMerger.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterMerger.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterMerger.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetClusterProducer(std::string name) {_cluster_producer = name;}

    ::cmtool::CMergeManager& GetManager() { return _mgr; }

    void SaveOutputCluster(bool doit=true) { _write_output = doit; }
    
  protected:

    bool _write_output;

    ::cmtool::CMergeManager _mgr;

    std::string _cluster_producer;

    ::cluster::CRUHelper _cru_helper;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
