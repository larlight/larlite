/**
 * \file DrawMCShower3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawMCShower3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWMCSHOWER3D_H
#define LARLITE_DRAWMCSHOWER3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryUtilities.h"
#include "DataFormat/mcshower.h"
#include "DrawShower3D.h"
#include "RecoBase3D.h"
/**
   \class DrawMCShower3D
   User defined class DrawMCShower3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class MCShower3D : public Shower3D {
    friend class DrawMCShower3D;
public:
    int _pdg;
    int pdg() {return _pdg;}
};


class DrawMCShower3D : public larlite::ana_base, public RecoBase3D<MCShower3D> {

public:

    /// Default constructor
    DrawMCShower3D();

    /// Default destructor
    ~DrawMCShower3D();

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

    MCShower3D getMCShower3D(larlite::mcshower shower);

};

} // evd

#endif
/** @} */ // end of doxygen group

