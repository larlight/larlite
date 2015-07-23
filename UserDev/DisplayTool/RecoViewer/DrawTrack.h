/**
 * \file DrawTrack.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class DrawTrack
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWTRACK_H
#define LARLITE_DRAWTRACK_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/track.h"
/**
   \class DrawTrack
   User defined class DrawTrack ... these comments are used to generate
   doxygen documentation!
 */

typedef std::vector< std::pair<float,float> > track2d;

namespace evd{

  class DrawTrack : public larlite::ana_base {

  public:

    /// Default constructor
    DrawTrack();

    /// Default destructor
    ~DrawTrack();

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


    const std::vector< track2d >   & getTracksByPlane(unsigned int p) const;
    // int getNTracks() const;
    // const track2d getTrack(unsigned int plane, unsigned int index) const;

  private:
    
    const larutil::Geometry * geoService;
    const larutil::GeometryUtilities * geoUtils;

    std::string producer;

    // Showers get drawn as cones.  Need a start point, and start direction
    // Also need an opening angle and the length
    // Return these as shower2d objects
    // This shows how to handle abstract objects in the viewer

    // Internally, keep the showers sorted by plane
    std::vector<std::vector< track2d > > * trackVectorByPlane;

    track2d getTrack2d(larlite::track track, unsigned int plane);

  };

} // evd

#endif
/** @} */ // end of doxygen group 

