/**
 * \file DrawPFParticle3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawPFParticle3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef LARLITE_DRAWPFPARTICLE3D_H
#define LARLITE_DRAWPFPARTICLE3D_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/spacepoint.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/seed.h"
#include "Cluster3DRecoUtil/Base/Cluster3DParams.h"
#include "Cluster3DRecoUtil/Base/CRU3DHelper.h"
#include "Cluster3DRecoUtil/Alg/Default3DParamsAlg.h"
#include "RecoBase3D.h"

/**
   \class DrawPFParticle3D
   User defined class DrawPFParticle3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {


class PFPart3D {

friend class DrawPFParticle3D;

public:

    cluster3D::cluster3D_params params(){return _params;}
    const std::vector<cluster3D::Point3D> points(){return _params.point_vector;}


private:

    cluster3D::cluster3D_params _params;

};


class DrawPFParticle3D : public larlite::ana_base, public RecoBase3D<PFPart3D> {

public:

    /// Default constructor
    DrawPFParticle3D();

    /// Default destructor
    ~DrawPFParticle3D();

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

    cluster3D::CRU3DHelper _cru3d_helper;
    cluster3D::Default3DParamsAlg _params_alg;

};

} // evd

#endif
/** @} */ // end of doxygen group

