#ifndef ERTOOL_GEOVIEWERERTOOLBACKEND_CXX
#define ERTOOL_GEOVIEWERERTOOLBACKEND_CXX

#include "GeoViewerERToolBackend.h"

namespace ertool {

  GeoViewerERToolBackend::GeoViewerERToolBackend()
    : ::geoalgo::GeoObjCollection()
  {}

  void GeoViewerERToolBackend::Add( const ::ertool::ParticleGraph& particles,
				    const ::ertool::EventData& data,
				    bool randColors) 
  {
    // Take particles, loop over them.
    // Make them into a LineSegments
    // give them to the collection

    // If randColors is tue, then don't pre-define color type ofr EventData objects
    // that have not yet been matched with particles
    std::string shr_col = "";
    std::string trk_col = "";
    std::string vtx_col = "";
    if (!randColors){
      shr_col = "grey";
      trk_col = "grey";
      vtx_col = "grey";
    }

    static TDatabasePDG db_s;
    //static std::map<int,std::string> part_name_s;
    static std::map<int,TParticlePDG*> part_map_s;

    std::vector<std::vector<bool> > used_obj(::ertool::kINVALID_RECO_TYPE, std::vector<bool>());
    used_obj[ ::ertool::kTrack  ].resize( data.Track().size(),  false );
    used_obj[ ::ertool::kShower ].resize( data.Shower().size(), false );

    for (auto &node_id : particles.GetParticles()) {

      auto const& p = particles.GetParticle(node_id);
      
      if(part_map_s.find(p.PdgCode()) == part_map_s.end()) 
	part_map_s.insert(std::make_pair(p.PdgCode(),db_s.GetParticle(p.PdgCode())));

	std::string name="";
	if(part_map_s[p.PdgCode()]) name = part_map_s[p.PdgCode()]->GetName();

	// If this particle has associated EventData, show it as well (if name is non-empty)
	if (p.RecoID() != kINVALID_RECO_ID){
	  if(!name.empty()) {
	    if (p.RecoType() == ::ertool::RecoType_t::kShower)
	      GeoObjCollection::Add( data.Shower(p.RecoID()), name , "blue");
	    if (p.RecoType() == ::ertool::RecoType_t::kTrack){
	      if (data.Track(p.RecoID()).size() >= 2)
	      GeoObjCollection::Add( data.Track(p.RecoID()), name, "red");
	    }
	  }
	  used_obj[ p.RecoType() ][ p.RecoID() ] = true;
	}else if(!name.empty()) {
	  //geoalgo::LineSegment_t part( daughter->Vertex(), daughter->Vertex() + (daughter->Momentum() * (30./daughter->Momentum().Length())) );
	  geoalgo::Vector_t part( p.Vertex() );
	  GeoObjCollection::Add( part, name, "black" );
	}
    }


    // Process all unassociated objects
    // Tracks
    for(size_t trk_index = 0; trk_index < data.Track().size(); ++trk_index) {
      if( !(used_obj[ ::ertool::RecoType_t::kTrack ][ trk_index ]) ){
	if( data.Track( trk_index ).size() > 2 )
	  GeoObjCollection::Add( data.Track( trk_index ), Form("un-tagged (%zu)",trk_index), shr_col.c_str() );
      }
    }
    // Showers
    for(size_t shr_index = 0; shr_index < data.Shower().size(); ++shr_index) {
      if( !(used_obj[ ::ertool::RecoType_t::kShower ][ shr_index ]) )
	GeoObjCollection::Add( data.Shower( shr_index ), Form("un-tagged (%zu)",shr_index), shr_col.c_str() );
    }
    return;
  }

}

#endif
