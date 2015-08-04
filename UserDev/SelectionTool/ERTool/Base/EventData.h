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
  namespace io {
    class IOHandler;
    class EmptyInput;
  }
}

namespace ertool {
  /**
     \class EventData
     @brief Data holder class to contain full information, to be used by AlgoX and FilterX
     Contains a vector of showers, tracks, and vertex points. Manager will change the Shower
     array contents for each possible combination of showers while tracks and vertex points are
     left unchanged. This class instance is provided to AlgoX and FilterX.
  */
  class EventData {
    friend class Manager;
    friend class io::IOHandler;
    friend class io::EmptyInput;
  public:
    
    /// Default constructor
    EventData();
    
    /// Default destructor
    virtual ~EventData(){}

    //
    // Getters
    //
    
    /// One shower object getter from Particle ID
    const ertool::Shower& Shower (const RecoID_t& id) const;
    /// One shower object getter from Particle
    const ertool::Shower& Shower (const Particle& p) const;

    /// One shower object getter from Particle ID
    const ertool::Track& Track (const RecoID_t& id) const;
    /// One shower object getter from Particle
    const ertool::Track& Track (const Particle& p) const;

    /// RecoInputID getter
    const ertool::RecoInputID_t& InputID(const RecoType_t& reco_type,
					 const RecoID_t&   reco_id) const;
    /// RecoInputID getter
    const ertool::RecoInputID_t& InputID(const Particle& p) const;

    /// All showers getter
    const std::vector< ertool::Shower >& Shower() const { return _shower_v; }
    /// All tracks getter
    const std::vector< ertool::Track >& Track() const { return _track_v;  }
    
    const unsigned int Event_ID() const { return _event_id;  }
    const unsigned int Run()      const { return _run     ;  }
    const unsigned int SubRun()   const { return _subrun  ;  }

    /// Function to set event id, run and subrun
    void SetID(unsigned int evID,
	       unsigned int runID,
	       unsigned int subrunID);
  protected:

    //
    // Setters
    //
    void Add(const ertool::Shower& obj, const RecoInputID_t& id);
    void Add(const ertool::Track&  obj, const RecoInputID_t& id);
#ifndef __CINT__
    void Emplace(const ertool::Shower&& obj, const RecoInputID_t&& id);
    void Emplace(const ertool::Track&&  obj, const RecoInputID_t&& id);
#endif
    /// Reset function
    void Reset();

    /// Showers
    std::vector<ertool::Shower> _shower_v;
    /// Input ID for showers
    std::vector<ertool::RecoInputID_t> _shower_id_v;
    /// Tracks
    std::vector<ertool::Track>  _track_v;
    /// Input ID for tracks
    std::vector<ertool::RecoInputID_t> _track_id_v;

    unsigned int _event_id;
    unsigned int _run;
    unsigned int _subrun;
  };
}

#endif
/** @} */ // end of doxygen group 

