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
#include "EMShowerTools/EMShowerProfile.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/EventData.h"
#include "FhiclLite/PSet.h"

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

    void Add(const ParticleGraph& particles,
	     const EventData& data,
	     bool randColors);
    
    /// Function to accept fclite::PSet
    virtual void AcceptPSet(const ::fcllite::PSet& cfg);

  private:
    
    EMShowerProfile _shrProfiler;

    // show cosmics?
    bool _show_cosmics;
    
  };
}

#endif
/** @} */ // end of doxygen group 
  
