/**
 * \file ClusterMergerArray.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class ClusterMergerArray
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_CLUSTERMERGERARRAY_H
#define RECOTOOL_CLUSTERMERGERARRAY_H

#include "Analysis/ana_base.h"
#include "CMergeHelper.h"
#include "ClusterRecoUtil/CRUHelper.h"

namespace larlite {
  /**
     \class ClusterMergerArray
     User custom analysis class made by kazuhiro
   */
  class ClusterMergerArray : public ana_base{
  
  public:

    /// Default constructor
    ClusterMergerArray();

    /// Default destructor
    virtual ~ClusterMergerArray(){}

    /** IMPLEMENT in ClusterMergerArray.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterMergerArray.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterMergerArray.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer(std::string name) {_input_producer = name;}

    void SetOutputProducer(std::string name) {_output_producer = name;}

    ::cmtool::CMergeManager& GetManager(size_t mgr_id=0);

    void SaveOutputCluster(bool doit=true) { _write_output = doit; }
    
  protected:

    bool _write_output;

    std::string _input_producer;

    std::string _output_producer;

    ::cmtool::CMergeHelper _merge_helper;

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
