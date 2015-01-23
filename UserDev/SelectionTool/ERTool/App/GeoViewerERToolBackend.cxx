#ifndef ERTOOL_GEOVIEWERERTOOLBACKEND_CXX
#define ERTOOL_GEOVIEWERERTOOLBACKEND_CXX

#include "GeoViewerERToolBackend.h"

namespace ertool {

  GeoViewerERToolBackend::GeoViewerERToolBackend()
    : ::geoalgo::GeoObjCollection()
  {}

  void GeoViewerERToolBackend::Add( const ::ertool::ParticleSet& particles,
				    const ::ertool::EventData& data) 
  {
    // Take particles, loop over them.
    // Make them into a LineSegments
    // give them to the collection

    static TDatabasePDG db_s;
    static std::map<int,std::string> part_name_s;

    std::vector<std::vector<bool> > used_obj(Particle::kTypeMax, std::vector<bool>());
    used_obj[ Particle::kTrack  ].resize( data.Track().size(),  false );
    used_obj[ Particle::kShower ].resize( data.Shower().size(), false );

    for (auto &p : particles){
      geoalgo::LineSegment_t part( p.Vertex(), p.Vertex() + (p.Momentum() * (30./p.Momentum().Length())) );
      if(part_name_s.find(p.PdgCode()) == part_name_s.end())
	part_name_s[p.PdgCode()] = db_s.GetParticle(p.PdgCode())->GetName();
      //Add( part, std::to_string(p.PdgCode() ), "black" );
      GeoObjCollection::Add( part, part_name_s[p.PdgCode()], "black" );
      // If this particle has associated EventData, show it as well
      if (p.RecoObjID() != -1){
	if (p.Type() == ::ertool::Particle::kShower)
	  GeoObjCollection::Add( data.Shower(p.RecoObjID()), "", "blue");
	if (p.Type() == ::ertool::Particle::kTrack){
	  if (data.Track(p.RecoObjID()).size() >= 2)
	    GeoObjCollection::Add( data.Track(p.RecoObjID()), "", "red");
	}
	used_obj[ p.Type() ][ p.RecoObjID() ] = true;
      }
    }

    // Process all unassociated objects
    for(size_t trk_index = 0; trk_index < data.Track().size(); ++trk_index) {
      
      if( used_obj[ Particle::kTrack ][ trk_index ] ) continue;

      if( data.Track( trk_index ).size() < 2 ) continue;

      GeoObjCollection::Add( data.Track( trk_index ), "un-tagged", "black" );
      
    }

    for(size_t shr_index = 0; shr_index < data.Shower().size(); ++shr_index) {
      
      if( used_obj[ Particle::kShower ][ shr_index ] ) continue;

      GeoObjCollection::Add( data.Shower( shr_index ), "un-tagged", "black" );
      
    }

    for(size_t vtx_index = 0; vtx_index < data.Vertex().size(); ++vtx_index) {
      
      GeoObjCollection::Add( data.Vertex( vtx_index ), "Vertex", "black" );
      
    }

    return;
  }

}

#endif
