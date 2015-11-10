/**
 * \file DrawTrack3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawTrack3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWTRACK3D_H
#define LARLITE_DRAWTRACK3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/track.h"

#include "RecoBase3D.h"
/**
   \class DrawTrack3D
   User defined class DrawTrack3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class Track3D {
public:
    std::vector<TVector3 > _track;
    const std::vector<TVector3 > & track() {return _track;}
    // const std::vector<std::vector<float> > & direction() {return _track;}
};


class DrawTrack3D : public larlite::ana_base, public RecoBase3D<Track3D> {

public:

    /// Default constructor
    DrawTrack3D();

    /// Default destructor
    ~DrawTrack3D();

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

    Track3D getTrack3d(larlite::track track);

};

} // evd

#endif
/** @} */ // end of doxygen group

