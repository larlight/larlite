/**
 * \file DrawVertex3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawVertex3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWVERTEX3D_H
#define LARLITE_DRAWVERTEX3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/vertex.h"
#include "RecoBase3D.h"
/**
   \class DrawSeed3D
   User defined class DrawSeed3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class Vertex3D {

friend class DrawVertex3D;

public:
    float X(){return _x;}
    float Y(){return _y;}
    float Z(){return _z;}

private:
    float _x;
    float _y;
    float _z;
};


class DrawVertex3D : public larlite::ana_base, public RecoBase3D<Vertex3D> {

public:

    /// Default constructor
    DrawVertex3D();

    /// Default destructor
    ~DrawVertex3D();

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

