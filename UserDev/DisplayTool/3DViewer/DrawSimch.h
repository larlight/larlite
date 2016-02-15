/**
 * \file DrawSimch.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawSimch
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWSIMCH_H
#define LARLITE_DRAWSIMCH_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/simch.h"
#include "Cluster3DRecoUtil/Base/Cluster3DParams.h"

#include "RecoBase3D.h"
/**
   \class DrawSimch
   User defined class DrawSimch ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

typedef cluster3D::Point3D Spacepoint3D;

class DrawSimch : public larlite::ana_base, public RecoBase3D<Spacepoint3D> {

public:

    /// Default constructor
    DrawSimch();

    /// Default destructor
    ~DrawSimch();

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

