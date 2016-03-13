/**
 * \file DrawOpflash3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawOpflash3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWOPFLASH3D_H
#define LARLITE_DRAWOPFLASH3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/opflash.h"
#include "TVector3.h"

#include "RecoBase3D.h"
/**
   \class DrawOpflash3D
   User defined class DrawOpflash3D ... these comments are used to generate
   doxygen documentation!
 */


namespace evd {


class Opflash3D {

friend class DrawOpflash3D;

public:

    float y(){return _y;}
    float y_width(){return _y_width;}
    float z(){return _z;}
    float z_width(){return _z_width;}
    float time(){return _time;}
    float time_width(){return _time_width;}

private:
    float _y;
    float _y_width;
    float _z;
    float _z_width;
    float _time;
    float _time_width;
};


class DrawOpflash3D : public larlite::ana_base, public RecoBase3D<Opflash3D> {

public:

    /// Default constructor
    DrawOpflash3D();

    /// Default destructor
    ~DrawOpflash3D();

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

