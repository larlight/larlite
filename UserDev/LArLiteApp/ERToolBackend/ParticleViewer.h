/**
 * \file ParticleViewer.h
 *
 * \ingroup SPTApp
 * 
 * \brief Class def header for a class ParticleViewer
 *
 * @author david
 */

/** \addtogroup SPTApp

    @{*/
#ifndef PARTICLEVIEWER_H
#define PARTICLEVIEWER_H

#include <iostream>
#include "GeoAlgo/GeoObjCollection.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/EventData.h"

namespace larlite {

  class ParticleViewer{
    /**
       \class ParticleViewer
       User defined class ParticleViewer ... these comments are used to generate
       doxygen documentation!
    */
    
    
  public:
    
    /// Default constructor
    ParticleViewer(){};
    
    /// Default destructor
    virtual ~ParticleViewer(){};
    
    void addParticles( const ::ertool::ParticleSet& particles,
		       geoalgo::GeoObjCollection& collection) const;

    void addParticles( const ::ertool::ParticleSet& particles,
		       const ::ertool::EventData& eventData,
		       geoalgo::GeoObjCollection& collection) const;

    void addEventData( const ::ertool::EventData& eventData,
		       geoalgo::GeoObjCollection& collection) const;
    
  private:
    
    ::ertool::EventData _data;

  };
}

#endif
/** @} */ // end of doxygen group 
  
