/**
 * \file GeoViewerERToolBackend.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class GeoViewerERToolBackend
 *
 * @author david C., kazu T.
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_GEOVIEWERERTOOLBACKEND_H
#define ERTOOL_GEOVIEWERERTOOLBACKEND_H

#include <iostream>
#include <map>
#include <TDatabasePDG.h>
#include "GeoAlgo/GeoObjCollection.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/EventData.h"

namespace ertool {

  class GeoViewerERToolBackend : public ::geoalgo::GeoObjCollection {
  /**
     \class GeoViewerERToolBackend
  */
    
  public:
    
    /// Default constructor
    GeoViewerERToolBackend();
    
    /// Default destructor
    virtual ~GeoViewerERToolBackend(){}

    void Add(const ParticleSet& particles,
	     const EventData& data);
    
  };
}

#endif
/** @} */ // end of doxygen group 
  
