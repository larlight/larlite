#ifndef PARTICLEVIEWER_CXX
#define PARTICLEVIEWER_CXX

#include "ParticleViewer.h"

namespace larlite {

  void ParticleViewer::addParticles( const ::ertool::ParticleSet& particles,
				     geoalgo::GeoObjCollection& collection) const

  {

    // Take particles, loop over them.
    // Make them into a LineSegments
    // give them to the collection
    for (auto &p : particles){
      // Different types of particles should be represented differently
      geoalgo::LineSegment_t part( p.Vertex(), p.Vertex() + (p.Momentum() * (30./p.Momentum().Length())) );
      collection.Add( part, std::to_string(p.PdgCode() ) );
    }
    return;
  }


  void ParticleViewer::addParticles( const ::ertool::ParticleSet& particles,
				     const ::ertool::EventData& eventData,
				     geoalgo::GeoObjCollection& collection) const
    
  {
    
    
    // Take particles, loop over them.
    // Make them into a LineSegments
    // give them to the collection
    for (auto &p : particles){
      geoalgo::LineSegment_t part( p.Vertex(), p.Vertex() + (p.Momentum() * (30./p.Momentum().Length())) );
      collection.Add( part, std::to_string(p.PdgCode() ), "red" );
      // If this particle has associated EventData, show it as well
      if (p.RecoObjID() != -1){
	if (p.Type() == ::ertool::Particle::RecoObjType_t::kShower)
	  collection.Add( eventData.getShowers().at(p.RecoObjID()), "Shower", "blue");
	if (p.Type() == ::ertool::Particle::RecoObjType_t::kTrack){
	  if (eventData.getTracks().at(p.RecoObjID()).size() > 2)
	    collection.Add( eventData.getTracks().at(p.RecoObjID()), "track", "red");
	}
      }
    }
    return;
  }


  void ParticleViewer::addEventData( const ::ertool::EventData& eventData,
				     geoalgo::GeoObjCollection& collection) const

  {


    // Get showers
    auto shrs = eventData.getShowers();
    auto trks = eventData.getTracks();
    for (auto& s: shrs)
      collection.Add( s, "SHOWER");
    for (auto& t: trks){
      if (t.size() >2)
	collection.Add( t, "TRACK", "black");
    }
    
    return;
  }
    
}

#endif
