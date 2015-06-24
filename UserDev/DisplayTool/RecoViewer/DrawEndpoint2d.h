/**
 * \file DrawEndpoint2d.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class DrawEndpoint2d
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/

#ifndef LARLITE_DRAWENDPOINT2D_H
#define LARLITE_DRAWENDPOINT2D_H

#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"

namespace larlite {
  /**
     \class DrawEndpoint2d
     User custom analysis class made by SHELL_USER_NAME
   */
  class DrawEndpoint2d : public ana_base{
  
  public:

    /// Default constructor
    DrawEndpoint2d();

    /// Default destructor
    virtual ~DrawEndpoint2d();

    /** IMPLEMENT in DrawEndpoint2d.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DrawEndpoint2d.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in DrawEndpoint2d.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void setProducer(std::string s){producer = s;}


    const std::vector<int>   & getWireByPlane(unsigned int p) const;
    const std::vector<float> & getTimeByPlane(unsigned int p) const;

    std::vector<float> GetWireRange(unsigned int p);
    std::vector<float> GetTimeRange(unsigned int p);

  protected:
    
  private:

    const larutil::Geometry * geoService;

    std::string producer;


    // Internally, keep the vertices sorted by plane
    std::vector<std::vector<int>   > * wireByPlane;
    std::vector<std::vector<float> > * timeByPlane;


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
