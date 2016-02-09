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
#ifndef LARLITE_DRAWSPACEPOINT3D_H
#define LARLITE_DRAWSPACEPOINT3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/spacepoint.h"
#include "Cluster3DRecoUtil/Base/Cluster3DParams.h"

#include "RecoBase3D.h"
/**
   \class DrawSpacepoint3D
   User defined class DrawSpacepoint3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

typedef cluster3D::Point3D Spacepoint3D;

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

