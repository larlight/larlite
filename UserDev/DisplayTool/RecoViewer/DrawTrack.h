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
#ifndef EVD_LARLITE_DRAWTRACK_H
#define EVD_LARLITE_DRAWTRACK_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "DataFormat/track.h"

#include "RecoBase.h"
/**
   \class DrawTrack
   User defined class DrawTrack ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2D;

namespace evd {

class Track2D {
public:
    std::vector<std::pair<float, float> > _track;
    const std::vector<std::pair<float, float> > & track() {return _track;}
    const std::vector<std::pair<float, float> > & direction() {return _track;}
};

// typedef std::vector<std::pair<float, float> > Track2D;

class DrawTrack : public larlite::ana_base, public RecoBase<Track2D> {

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

    Track2D getTrack2D(larlite::track track, unsigned int plane);

private:


};

} // evd

#endif
/** @} */ // end of doxygen group

