/**
 * \file DrawSpacepoint3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawSpacepoint3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWSPACEPOINT_H
#define LARLITE_DRAWSPACEPOINT_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/spacepoint.h"
#include "TVector3.h"

#include "RecoBase3D.h"
/**
   \class DrawSpacepoint3D
   User defined class DrawSpacepoint3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

typedef TVector3 Spacepoint3D;

// class Shower3D {

// friend class DrawSpacepoint3D;

// public:
//     // std::vector<TVector3 > _track;
//     // const std::vector<TVector3 > & shower() {return _track;}
//     // const std::vector<std::vector<float> > & direction() {return _track;}
//     TVector3 start_point(){return _start_point;}
//     TVector3 direction(){return _direction;}
//     float length(){return _length;}
//     float opening_angle(){return _opening_angle;}

// private:
//     TVector3 _start_point;
//     TVector3 _direction;
//     float _length;
//     float _opening_angle;
// };


class DrawSpacepoint3D : public larlite::ana_base, public RecoBase3D<Spacepoint3D> {

public:

    /// Default constructor
    DrawSpacepoint3D();

    /// Default destructor
    ~DrawSpacepoint3D();

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


};

} // evd

#endif
/** @} */ // end of doxygen group

