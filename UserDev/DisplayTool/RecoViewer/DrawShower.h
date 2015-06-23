/**
 * \file DrawShower.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class DrawShower
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWSHOWER_H
#define LARLITE_DRAWSHOWER_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/shower.h"
#include "Shower2d.h"
/**
   \class DrawShower
   User defined class DrawShower ... these comments are used to generate
   doxygen documentation!
 */



namespace evd{

  class DrawShower : public larlite::ana_base {

  public:

    /// Default constructor
    DrawShower();

    /// Default destructor
    ~DrawShower();

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


    const std::vector<Shower2d>   & getShowersByPlane(unsigned int p) const;


  private:
    
    const larutil::Geometry * geoService;
    const larutil::GeometryUtilities * geoUtils;

    std::string producer;

    // Showers get drawn as cones.  Need a start point, and start direction
    // Also need an opening angle and the length
    // Return these as shower2d objects
    // This shows how to handle abstract objects in the viewer

    // Internally, keep the showers sorted by plane
    std::vector<std::vector<Shower2d> > * showerVectorByPlane;

    Shower2d getShower2d(larlite::shower shower, unsigned int plane);

  };

} // evd

#endif
/** @} */ // end of doxygen group 

