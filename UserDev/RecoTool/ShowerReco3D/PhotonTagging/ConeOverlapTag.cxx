#ifndef LARLITE_CONEOVERLAPTAG_CXX
#define LARLITE_CONEOVERLAPTAG_CXX

#include "ConeOverlapTag.h"

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

namespace larlite {

  bool ConeOverlapTag::initialize() {

    auto geomH = larutil::GeometryHelper::GetME();
    
    _w2cm = geomH->WireToCm();
    _t2cm = geomH->TimeToCm();

    return true;
  }
  
  bool ConeOverlapTag::analyze(storage_manager* storage) {

    // procedure followed by this module.
    // 1) load reconstructed showers.
    // 2) for each plane, identify area covered by shower on the plane (cone projection)
    // 3) identify un-tagged photon clusters which overlap the projected area
    // 4) add hits from overlapping photons to shower clusters IFF hit not already included.

    // grab already reconstructed showers
    auto ev_shower = storage->get_data<event_shower>(_shower_producer);
    // grab associated clusters
    larlite::event_cluster* ev_cluster = nullptr;
    auto const& ass_shr_clus_v = storage->find_one_ass(ev_shower->id(), ev_cluster, ev_shower->name());
    // grab associated hits
    larlite::event_hit* ev_hit_shower = nullptr;
    auto const& ass_clus_hit_v = storage->find_one_ass(ev_cluster->id(), ev_hit_shower, ev_cluster->name());

    // grab reconstructed photon clusters
    auto ev_photon = storage->get_data<event_cluster>(_photon_producer);
    // grab associated hits
    larlite::event_hit* ev_hit_photon = nullptr;
    auto const& ass_photon_hit_v = storage->find_one_ass(ev_photon->id(), ev_hit_photon, ev_photon->name());

    // if hits associated with showers and those associated with photon clusters
    // are from different producers -> exit.
    // it would be impossible to check whether we are double-counting hits.

    if (ev_hit_photon->name() != ev_hit_shower->name() ){
      print(larlite::msg::kERROR, __FUNCTION__, Form("Shower and Photon hit producers different. Quit."));
      return true;
    }

    auto geomH = larutil::GeometryHelper::GetME();
    _photon_poly_v.clear();
    _photon_poly_v.resize(3);

    // create polygon objects for each photon cluster.
    for (size_t p=0; p < ev_photon->size(); p++){

      // get assciated hits
      auto const& photon_hit_idx_v = ass_photon_hit_v.at(p);
      
      // get polygon
      std::vector<larutil::Hit2D> photon_hit2d_v;
      for (auto const& hit_idx: photon_hit_idx_v){
	auto const& hit = ev_hit_photon->at(hit_idx);
	larutil::Hit2D hit2d;
	hit2d.plane = (unsigned char)hit.WireID().Plane;
	hit2d.w     = hit.WireID().Wire * _w2cm;
	hit2d.t     = hit.PeakTime() * _t2cm;
	photon_hit2d_v.push_back( hit2d );
      }// for all hits in photon cluster
      std::vector<const larutil::Hit2D*> polygonEdges;
      geomH->SelectPolygonHitList(photon_hit2d_v, polygonEdges, 1.0);
      //now making Polygon Object
      std::pair<float, float> tmpvertex;
      //make Polygon Object as in mac/PolyOverlap.cc
      std::vector<std::pair<float, float> > vertices;
      for (unsigned int i = 0; i < polygonEdges.size(); i++) {
	tmpvertex = std::make_pair( polygonEdges.at(i)->w,
				    polygonEdges.at(i)->t );
	vertices.push_back( tmpvertex );
      }// for all polygon edges
      Polygon2D thispoly( vertices );
      std::pair<size_t, Polygon2D> polyinfo(p, thispoly);
      _photon_poly_v.at( photon_hit2d_v.at(0).plane ).push_back( polyinfo );
      
    }// for all photon clusters
      

    // loop through reconstructed showers.
    for (size_t s=0; s < ev_shower->size(); s++) {

      auto const& shr = ev_shower->at(s);

      // get the hit indices assocaited with this reco'd shower
      // on the various planes
      std::vector< std::vector<unsigned int> > shr_hit_ass_idx_v;
      shr_hit_ass_idx_v.reserve(3); // one entry per plane
      // clusters assocaited with this shower:
      auto const& clus_idx_v = ass_shr_clus_v.at(s);
      for (auto const& clus_idx : clus_idx_v){
	// hits associated with this cluster
	auto const& hit_idx_v = ass_clus_hit_v.at(clus_idx);
	// find plane
	size_t pl = ev_hit_shower->at(hit_idx_v.at(0)).WireID().Plane;
	// loop over hits and add the indices:
	for (auto const& hit_idx : hit_idx_v)
	  shr_hit_ass_idx_v.at(pl).push_back(hit_idx);
      }// for all clusters associated to shower
      
      // project shower on the 3 planes
      projectShower(shr);
      
      // loop over 3 planes
      for (size_t pl=0; pl < 3; pl++) {

	auto const& shrPoly = _shr_polygon_v.at(pl);
	
	// loop over photons for this plane
	for (auto const& photonPoly : _photon_poly_v.at(pl) ) {
	  
	  // are they compatible?
	  if (shrPoly.PolyOverlap(photonPoly.second) == true) {
	    
	    // get set of hits to add (removing potential duplicates)
	    mergeHits( shr_hit_ass_idx_v.at(pl),
		       ass_photon_hit_v.at(photonPoly.first) );
	    
	  }// compatible showers

	}// for all photon clusters
	
      }// for all 3 planes
      
    }// for all reconstructed showers
    
    return true;
  }

  bool ConeOverlapTag::finalize() {

    return true;
  }

  // return a triangle for each plane
  void ConeOverlapTag::projectShower(const larlite::shower& shr) {

    auto geom  = larutil::Geometry::GetME();
    auto geomH = larutil::GeometryHelper::GetME();

    _shr_polygon_v.clear();
    _shr_polygon_v.resize(geom->Nplanes());

    auto const& start  = shr.ShowerStart();
    auto const& dir    = shr.Direction();
    auto const& oangle = shr.OpeningAngle();

    // end point, 1 meter away from start in shower direction [TEMPORARY]
    double shrLen = 100.; // cm
    auto const& end = start + shrLen * dir;

    for (size_t pl=0; pl < geom->Nplanes(); pl++){

      // vector of coordiantes with which to consutrct triangle polygon
      std::vector<std::pair<float,float> > triangle_coordinates;
      
      // project start point to plane coordinates
      auto const& start_pl = geomH->Point_3Dto2D( start, pl);
      // project end point to plane coordinates
      auto const& end_pl   = geomH->Point_3Dto2D( end  , pl);

      // create polygon vertex for triangle
      triangle_coordinates.push_back( std::pair<float,float>(start_pl.w, start_pl.t) );

      // figure out how far to go on each side.
      double shrWidth = shrLen * tan(oangle);

      // unlike length, width is not stretched or compressed on projection.
      // extend end-point "left" and "right" by one width to complete triangle
      // extend in direction perpendicular to line connecting start_pl and end_pl
      double slope = -1. / ( (end_pl.t - start_pl.t) / (end_pl.w - start_pl.t) );
      //double intrc = end_pl.t - slope * end_pl.w;

      // find triangle base coordinate points
      double pt1_w = end_pl.w + sqrt( shrWidth / (1 + slope*slope) );
      double pt1_t = end_pl.t + slope * sqrt( shrWidth / (1 + slope*slope) );
      triangle_coordinates.push_back( std::pair<float,float>( pt1_w, pt1_t) );

      double pt2_w = end_pl.w - sqrt( shrWidth / (1 + slope*slope) );
      double pt2_t = end_pl.t - slope * sqrt( shrWidth / (1 + slope*slope) );
      triangle_coordinates.push_back( std::pair<float,float>( pt2_w, pt2_t) );

      Polygon2D triangle(triangle_coordinates);
      _shr_polygon_v[pl] = triangle;
      

    }// for all 3 planes
    
    return;
  }

  void ConeOverlapTag::mergeHits(std::vector<unsigned int>& shr_hit_idx_v,
				 const std::vector<unsigned int>& photon_hit_idx_v) {

    // static copy of shower hit index vector to use for iteration
    std::vector<unsigned int> shr_hit_idx_v_old = shr_hit_idx_v;
    
    for(auto const& photon_hit_idx : photon_hit_idx_v) {
      for (auto const& shr_hit_idx : shr_hit_idx_v_old) {
	if (shr_hit_idx != photon_hit_idx)
	  shr_hit_idx_v.push_back(photon_hit_idx);
      }// for all shower hits
    }// for all photon hits

    return;
  }
   

}
#endif
