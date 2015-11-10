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

#include "RecoBase.h"
/**
   \class DrawTrack
   User defined class DrawTrack ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class Track2d {
public:
    std::vector<std::pair<float, float> > _track;
    const std::vector<std::pair<float, float> > & track() {return _track;}
    const std::vector<std::pair<float, float> > & direction() {return _track;}
};

// typedef std::vector<std::pair<float, float> > Track2d;

class DrawTrack : public larlite::ana_base, public RecoBase<Track2d> {

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


private:

    Track2d getTrack2d(larlite::track track, unsigned int plane);

};

} // evd

#endif
/** @} */ // end of doxygen group

