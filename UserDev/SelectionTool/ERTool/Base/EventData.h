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
#include "Vertex.h"
#include "UtilFunc.h"
//#include "BookKeeper.h"
namespace ertool {

  /**
     \class EventData
     @brief Data holder class to contain full information, to be used by AlgoX and FilterX
     Contains a vector of showers, tracks, and vertex points. Manager will change the Shower
     array contents for each possible combination of showers while tracks and vertex points are
     left unchanged. This class instance is provided to AlgoX and FilterX.
  */
  class EventData : public TObject {
    
  public:
    
    /// Default constructor
    EventData();
    
    /// Default destructor
    virtual ~EventData(){}

    //
    // Getters
    //
    const ::ertool::Shower& Shower (size_t id) const;
    const ::ertool::Track&  Track  (size_t id) const;
    const ::ertool::Vertex& Vertex (size_t id) const;
    std::vector< const ::ertool::Shower* > FilteredShower() const;
    std::vector< const ::ertool::Track*  > FilteredTrack() const;
    std::vector< const ::ertool::Vertex* > FilteredVertex() const;
    std::vector< const ::ertool::Shower* > Shower() const;
    std::vector< const ::ertool::Track*  > Track()  const;
    std::vector< const ::ertool::Vertex* > Vertex() const;
    const std::vector< ::ertool::Shower > AllShower() const { return _shower_v; }
    const std::vector< ::ertool::Track  > AllTrack()  const { return _track_v;  }
    const std::vector< ::ertool::Vertex > AllVertex() const { return _vertex_v; }
    std::vector<std::vector< const ::ertool::Shower* > > ShowerCombination (size_t n) const;
    std::vector<std::vector< const ::ertool::Track*  > > TrackCombination  (size_t n)  const;
    std::vector<std::vector< const ::ertool::Vertex* > > VertexCombination (size_t n) const;
    //
    // Setters
    //
    void Add(const ::ertool::Shower& obj);
    void Add(const ::ertool::Track&  obj);
    void Add(const ::ertool::Vertex& obj);

    void FilterShower (size_t id, bool filter=true);
    void FilterTrack  (size_t id, bool filter=true);
    void FilterVertex (size_t id, bool filter=true);

    /// Reset function
    void Reset();

  protected:

    /// Showers
    std::vector<ertool::Shower> _shower_v;
    /// Tracks
    std::vector<ertool::Track>  _track_v;
    /// Candidate event vertecies
    std::vector<ertool::Vertex> _vertex_v;

    ClassDef(EventData,1)
  };
}

#endif
/** @} */ // end of doxygen group 

