/**
 * \file ParticleConverter.h
 *
 * \ingroup SPTApp
 * 
 * \brief Class def header for a class ParticleConverter
 *
 * @author david
 */

/** \addtogroup SPTApp

    @{*/
#ifndef PARTICLECONVERTER_H
#define PARTICLECONVERTER_H

#include <iostream>
#include "ERTool/Base/RecoObjBase.h"
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/Track.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/EventData.h"
#include "DataFormat/mcpart.h"

namespace larlite {
  /**
     \class ParticleConverter
     Functions that take an mcparticle object and return the appropriate
     GeoAlgo object to be used for plotting and so on.
     doxygen documentation!
  */
  class ParticleConverter{
    
  public:
    
    /// Default constructor
    ParticleConverter(){};
    
    /// Default destructor
    virtual ~ParticleConverter(){};
    
    /// Transformer function
    ::ertool::RecoObjBase Convert(const mcpart part) const;
    
  };
}

#endif
/** @} */ // end of doxygen group 

