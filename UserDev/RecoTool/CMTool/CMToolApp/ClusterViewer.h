/**
 * \file ClusterViewer.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for a class ClusterViewer
 *
 * @author kazuhiro/kaleko
 */

/** \addtogroup CMTool

    @{*/

#ifndef RECOTOOL_CLUSTERVIEWER_H
#define RECOTOOL_CLUSTERVIEWER_H

#include "ClusterViewerAlgo.h"

#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/simch.h"
#include "Analysis/ana_base.h"
#include "ClusterRecoUtil/CRUHelper.h"
#include <TH2D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TPad.h>

namespace larlite{
  /**
     \class ClusterViewer
     User custom analysis class made by kazuhiro/kaleko
  */
  class ClusterViewer : public ana_base{
    
  public:
    
    /// Default constructor
    ClusterViewer();
    
    /// Default destructor
    virtual ~ClusterViewer(){};
    
    /** IMPLEMENT in ClusterViewer.cc!
	Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();
    
    /** IMPLEMENT in ClusterViewer.cc! 
	Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in ClusterViewer.cc! 
	Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Setter for a cluster type to be viewed
    virtual void SetClusterProducer(std::string name) { _cluster_producer = name;}

    /// Function to get number of clusters in the given plane
    size_t ClusterCount(UChar_t plane) {return _algo.ClusterCount(plane); }
    
    /// Function to draw a main canvas (where cluster hits & all-hits are shown)
    void DrawAllClusters() {_algo.DrawAllClusters();}

    /// Function to draw a secondary canvas (where individual cluster is shown)
    void DrawOneCluster(UChar_t plane, size_t index) {_algo.DrawOneCluster(plane,index);}

    /// Function to draw a tertiary canvas w/ two clusters on it
    void DrawTwoClusters(UChar_t plane, size_t index1, size_t index2) {_algo.DrawTwoClusters(plane,index1,index2);}

    /// Function to decide if to show MCshowers or hit-charge
    void SetDrawShowers(bool on);

    /// A function to decide if the hits COLZ canvas should be shown in log-z scale
    void SetHitsLogZ(bool flag);

    /// A function to decide if to show start & end points
    void SetDrawStartEnd(bool on) { _showStartEnd = on; }

  protected:
    /// Type of cluster to view
    std::string _cluster_producer;

    /// Actual viewer code
    ::cluster::ClusterViewerAlgo _algo;

    // McshowerLookback object
    //McshowerLookback _mcslb;

    //some maps that mcshowerlookback fills once per event
    std::map<UInt_t,UInt_t> _shower_idmap;
    std::map<UShort_t,larlite::simch> _simch_map;
    bool _showerColor;
    bool _hits_log_z;
    bool _showStartEnd; /// decide if tho show start and end points

    ::cluster::CRUHelper _cru_helper;
  };
  
}
#endif

/** @} */ // end of doxygen group 
