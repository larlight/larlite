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
#include "CMergeHelper.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"

namespace larlite {
  /**
     \class ClusterMerger
     User custom analysis class made by kazuhiro
   */
  class ClusterMerger : public ana_base{
  
  public:

    /// Default constructor
    ClusterMerger();

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

    void SetInputProducer(std::string name) {_input_producer = name;}

    void SetOutputProducer(std::string name) {_output_producer = name;}

    ::cmtool::CMergeManager& GetManager(size_t i=0) { return _merge_helper.GetManager(i); }

    size_t GetNumManager() { return _merge_helper.size(); }
    
    void SaveOutputCluster(bool doit=true) { _write_output = doit; }
    
  protected:

    bool _write_output;

    ::cmtool::CMergeHelper _merge_helper;

    std::string _input_producer;

    std::string _output_producer;

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
