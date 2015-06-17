/**
 * \file AnaBase.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AnaBase
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_ANABASE_H
#define ERTOOL_ANABASE_H

#include <iostream>
#include "UnitBase.h"
#include "EventData.h"
#include "Particle.h"
#include "ERException.h"

namespace ertool {

  class Manager;
  /**
     \class AnaBase
     User defined class ertool::AnaBase ... these comments are used to generate
     doxygen documentation!
  */
  class AnaBase : public UnitBase{
    friend class Manager;
  public:
    
    /// Default constructor
    AnaBase();
    
    /// Default destructor
    virtual ~AnaBase(){}

    virtual bool Analyze(const EventData& data,
			 const ParticleSet& ps);
    
  protected:

    void SetMCData(EventData& data, ParticleSet& ps);

    void UnsetMCData();

    const EventData&   MCEventData() const;

    const ParticleSet& MCParticleSet() const;

  private:

    EventData*   _mc_data;
    ParticleSet* _mc_ps;

  };
}

#endif
/** @} */ // end of doxygen group 

