/**
 * \file EventData.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class ertool::EventData and 
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_EVENTDATA_H
#define ERTOOL_EVENTDATA_H

#include <vector>
#include "Track.h"
#include "Shower.h"
//#include "BookKeeper.h"
namespace ertool {
  class Particle;
  
  class Manager;
  /**
     \class EventData
     @brief Data holder class to contain full information, to be used by AlgoX and FilterX
     Contains a vector of showers, tracks, and vertex points. Manager will change the Shower
     array contents for each possible combination of showers while tracks and vertex points are
     left unchanged. This class instance is provided to AlgoX and FilterX.
  */
  class EventData {
    friend class Manager;
  public:
    
    /// Default constructor
    EventData();
    
    /// Default destructor
    virtual ~EventData(){}

    //
    // Getters
    //
    
    /// One shower object getter from Particle ID
    const ::ertool::Shower& Shower (const RecoID_t& id);
    /// One shower object getter from Particle
    const ::ertool::Shower& Shower (const Particle& p);
    /// One shower object getter from Particle

    /// One shower object getter from Particle ID
    const ::ertool::Track& Track (const RecoID_t& id);
    /// One shower object getter from Particle
    const ::ertool::Track& Track (const Particle& p);

    /// All showers getter
    const std::vector< ::ertool::Shower >& Shower() const { return _shower_v; }
    /// All tracks getter
    const std::vector< ::ertool::Track >& Track() const { return _track_v;  }

  protected:

    //
    // Setters
    //
    void Add(const ::ertool::Shower& obj);
    void Add(const ::ertool::Track&  obj);
#ifndef __CINT__
    void Emplace(const ::ertool::Shower&& obj);
    void Emplace(const ::ertool::Track&&  obj);
#endif
    /// Reset function
    void Reset();

    /// Showers
    std::vector<ertool::Shower> _shower_v;
    /// Tracks
    std::vector<ertool::Track>  _track_v;
  };
}

#endif
/** @} */ // end of doxygen group 

