/**
 * \file DrawSpacepoint.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawSpacepoint
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWSPACEPOINT_H
#define EVD_DRAWSPACEPOINT_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/spacepoint.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"

#include "RecoBase.h"
/**
   \class DrawSpacepoint
   User defined class DrawSpacepoint ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {


class DrawSpacepoint : public larlite::ana_base, public RecoBase<Point2D> {

public:

    /// Default constructor
    DrawSpacepoint();

    /// Default destructor
    ~DrawSpacepoint();

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

