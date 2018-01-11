/**
 * \file ClusterMatcher.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class ClusterMatcher
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_CLUSTERMATCHER_H
#define RECOTOOL_CLUSTERMATCHER_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "CMTool/CMToolBase/CMatchManager.h"
#include "CMTool/CMToolBase/ClusterMaker.h"

namespace larlite {
  /**
     \class ClusterMatcher
     User custom analysis class made by kazuhiro
   */
  class ClusterMatcher : public ana_base{
  
  public:

    /// Default constructor
    ClusterMatcher();

    /// Default destructor
    virtual ~ClusterMatcher(){ delete _mgr; }

    /** IMPLEMENT in ClusterMatcher.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ClusterMatcher.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ClusterMatcher.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetClusterProducer(const std::string name) { _clus_producer = name; }
    void SetVertexProducer (const std::string name) { _vtx_producer  = name; }

   void SetOutputProducer(std::string name) {_output_producer = name;}

    void SaveOutputCluster(bool doit=true) { _write_output = doit; }

    ::cmtool::CMatchManager& GetManager() { return *_mgr; }

  protected:

    bool _write_output;

    ::cmtool::CMatchManager *_mgr;

    std::string _clus_producer;
    std::string _vtx_producer;

    std::string _output_producer;

    ::cluster::ClusterMaker _cluster_maker;

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
