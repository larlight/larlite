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
#include "ParticleGraph.h"

namespace ertool {
  //class ParticleGraph;
  //class EventData;

  
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
    AnaBase(const std::string& name="");
    
    /// Default destructor
    virtual ~AnaBase(){}

    virtual bool Analyze(const EventData& data,
			 const ParticleGraph& graph);
    
  protected:

    const EventData&   MCEventData() const;

    const ParticleGraph& MCParticleGraph() const;

  private:

    void SetMCData(const EventData& data, const ParticleGraph& graph);

    void UnsetMCData();

    const EventData*   _mc_data;
    const ParticleGraph* _mc_graph;

  };
}

#endif
/** @} */ // end of doxygen group 

