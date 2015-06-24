/**
 * \file DrawHit.h
 *
 * \ingroup RecoViewer
 * 
 * \brief Class def header for a class DrawHit
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWHIT_H
#define LARLITE_DRAWHIT_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"

/**
   \class DrawHit
   User defined class DrawHit ... these comments are used to generate
   doxygen documentation!
 */

namespace evd {

  class DrawHit : public larlite::ana_base {

  public:

    /// Default constructor
    DrawHit();

    /// Default destructor
    ~DrawHit();

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


    const std::vector<int>   & getWireByPlane(unsigned int p) const;
    const std::vector<float> & getHitStartByPlane(unsigned int p) const;
    const std::vector<float> & getHitEndByPlane(unsigned int p) const;

  private:
    
    const larutil::Geometry * geoService;

    std::string producer;

    // The hit data gets drawn as rectangles.  So we need to pass 
    // the data in a way that's easy to draw.  That turns out to be
    // (wire, start_time, end_time)
    // Pack it up into 3 vectors that go together, easiest to
    // convert to numpy and loop through.

    // Internally, keep the hits sorted by plane
    std::vector<std::vector<int>   > * wireByPlane;
    std::vector<std::vector<float> > * hitStartByPlane;
    std::vector<std::vector<float> > * hitEndByPlane;

  };

} // evd

#endif
/** @} */ // end of doxygen group 

