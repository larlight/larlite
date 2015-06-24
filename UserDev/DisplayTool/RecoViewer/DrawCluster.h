/**
 * \file DrawCluster.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class DrawCluster
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/

#ifndef LARLITE_DRAWCLUSTER_H
#define LARLITE_DRAWCLUSTER_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"

namespace evd {
  /**
     \class DrawCluster
     User custom analysis class made by SHELL_USER_NAME
   */
  class DrawCluster : public larlite::ana_base{
  
  public:

    /// Default constructor
    DrawCluster();

    /// Default destructor
    virtual ~DrawCluster();

    /** IMPLEMENT in DrawCluster.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DrawCluster.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(larlite::storage_manager* storage);

    /** IMPLEMENT in DrawCluster.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void setProducer(std::string s){producer = s;}

    int getNClustersByPlane(unsigned int p) const;

    const std::vector<int>   & getWireByPlaneAndCluster(unsigned int p, unsigned int c) const;
    const std::vector<float> & getHitStartByPlaneAndCluster(unsigned int p, unsigned int c) const;
    const std::vector<float> & getHitEndByPlaneAndCluster(unsigned int p, unsigned int c) const;

    std::vector<float> GetWireRange(unsigned int p);
    std::vector<float> GetTimeRange(unsigned int p);

  protected:
    
  private:

    const larutil::Geometry * geoService;

    std::string producer;

    // Internally, keep the hits sorted by cluster
    // For drawing purposes, need to know all of this hits
    // by plane but also by cluster.
    // Return at most 2 index objects (because I don't 
    // feel like writing the conversion for 3 index)
    // But store 3 index objects
    std::vector<std::vector<std::vector<int>   > > * wireByPlaneByCluster;
    std::vector<std::vector<std::vector<float> > > * hitStartByPlaneByCluster;
    std::vector<std::vector<std::vector<float> > > * hitEndByPlaneByCluster;

    // Store the bounding parameters of interest:
    // highest and lowest wire, highest and lowest time
    // Have to sort by plane still

    std::vector<std::vector<float> > wireRange;
    std::vector<std::vector<float> > timeRange;

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
