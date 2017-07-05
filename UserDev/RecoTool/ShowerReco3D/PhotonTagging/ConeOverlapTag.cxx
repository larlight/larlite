
#ifndef LARLITE_CONEOVERLAPTAG_CXX
#define LARLITE_CONEOVERLAPTAG_CXX

#include "ConeOverlapTag.h"

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

#include "DataFormat/shower.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/event_ass.h"

namespace larlite {

  ConeOverlapTag::ConeOverlapTag() {

    _name="ConeOverlapTag";

    _fout=0;

    _debug=false;

    auto geomH = larutil::GeometryHelper::GetME();
    
    _w2cm = geomH->WireToCm();
    _t2cm = geomH->TimeToCm();

    _shrLen = 100.;
    _width  = -1;
    _tick_offset = 800;
    _max_slope_angle = 90;

  }

  bool ConeOverlapTag::initialize() {

    return true;
  }
  
  bool ConeOverlapTag::analyze(storage_manager* storage) {

    if (_debug) { std::cout << "ANALYZE module" << std::endl; }

    // procedure followed by this module.
    // 1) load reconstructed showers.
    // 2) for each plane, identify area covered by shower on the plane (cone projection)
    // 3) identify un-tagged photon clusters which overlap the projected area
    // 4) add hits from overlapping photons to shower clusters IFF hit not already included.

    // grab already reconstructed showers
    auto ev_shower = storage->get_data<event_shower>(_shower_producer);
    if (_debug) { std::cout << "event has " << ev_shower->size() << " showers" << std::endl; }
    if (_debug) { std::cout << "Shower producer : " << ev_shower->name() << std::endl; }
    // grab associated pfparticles
    larlite::event_pfpart* ev_pfpart = nullptr;
    // grab associated clusters
    larlite::event_cluster* ev_cluster = nullptr;
    // grab associated hits
    larlite::event_hit* ev_hit_shower = nullptr;

    // grab reconstructed photon clusters
    auto ev_photon = storage->get_data<event_cluster>(_photon_producer);
    // grab associated hits
    larlite::event_hit* ev_hit_photon = nullptr;

    // create a new PFParticle to store the newly clustered showers
    auto  ev_pfpart_new  = storage->get_data<event_pfpart>(_out_pfpart_producer);
    // create new clusters for the newly formed shower clusters
    auto ev_cluster_new = storage->get_data<event_cluster>(_out_cluster_producer);
    // create a new pfpart -> cluster association
    larlite::event_ass* ev_pfpart_cluster_ass_v_new = storage->get_data<event_ass>( ev_pfpart_new->name() );
    // create a new cluster -> hit association
    larlite::event_ass* ev_cluster_hit_ass_v_new    = storage->get_data<event_ass>( ev_cluster_new->name() );

    //set event ID through storage manager
    storage->set_id(storage->run_id(),storage->subrun_id(),storage->event_id());

    // if no imput showers -> quit
    if ( !ev_shower || (ev_shower->size() == 0) )
      return true;

    auto const& ass_shr_pfpart_v = storage->find_one_ass(ev_shower->id(), ev_pfpart, ev_shower->name());
    if (_debug) { std::cout << "PFPart producer : " << ev_pfpart->name() << std::endl; }
    auto const& ass_pfpart_clus_v = storage->find_one_ass(ev_pfpart->id(), ev_cluster, ev_pfpart->name());
    if (_debug) { std::cout << "Cluster producer : " << ev_cluster->name() << std::endl; }
    auto const& ass_clus_hit_v = storage->find_one_ass(ev_cluster->id(), ev_hit_shower, ev_cluster->name());
    if (_debug) { std::cout << "Shower Hit producer : " << ev_hit_shower->name() << std::endl; }
    auto const& ass_photon_hit_v = storage->find_one_ass(ev_photon->id(), ev_hit_photon, ev_photon->name());
    if (_debug) { std::cout << "Photon Hit producer : " << ev_hit_photon->name() << std::endl; }

    // vector to hold associated hits to cluster
    std::vector< std::vector< unsigned int> > clus_hit_ass_holder_new;
    // vector to hold pfpart -> cluster ass
    std::vector< std::vector< unsigned int> > pfpart_clus_ass_holder_new;

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

      // is this photon on the collection-plane? if not skip
      if (ev_hit_photon->at( photon_hit_idx_v[0] ).WireID().Plane != 2) continue;
      
      // create linearity objects
      std::vector<double> hit_w_v;
      std::vector<double> hit_t_v;
      
      // get polygon
      std::vector<larutil::Hit2D> photon_hit2d_v;
      for (auto const& hit_idx: photon_hit_idx_v){
	auto const& hit = ev_hit_photon->at(hit_idx);
	larutil::Hit2D hit2d;
	hit2d.plane  = (unsigned char)hit.WireID().Plane;
	hit2d.w      = hit.WireID().Wire * _w2cm;
	hit2d.t      = (hit.PeakTime() - _tick_offset) * _t2cm;
	hit2d.charge = hit.Integral();
	hit2d.peak   = hit.PeakAmplitude();
	photon_hit2d_v.push_back( hit2d );
	hit_w_v.push_back( hit2d.w);
	hit_t_v.push_back( hit2d.t);
      }// for all hits in photon cluster
      std::vector<const larutil::Hit2D*> polygonEdges;
      geomH->SelectPolygonHitList(photon_hit2d_v, polygonEdges, 0.95);
      //now making Polygon Object
      std::pair<float, float> tmpvertex;
      //make Polygon Object as in mac/PolyOverlap.cc
      std::vector<std::pair<float, float> > vertices;
      for (unsigned int i = 0; i < polygonEdges.size(); i++) {
	tmpvertex = std::make_pair( polygonEdges.at(i)->w,
				    polygonEdges.at(i)->t );
	vertices.push_back( tmpvertex );
      }// for all polygon edges
      twodimtools::Poly2D    thispoly( vertices );
      twodimtools::Linearity thislin( hit_w_v, hit_t_v );
      std::pair<size_t, twodimtools::Poly2D>    polyinfo(p, thispoly);
      _photon_poly_v.at( photon_hit2d_v.at(0).plane ).push_back( polyinfo );
      _photon_lin_map[ p ] = thislin;
      
    }// for all photon clusters
      
    if (_debug) { std::cout << "now loop through showers" << std::endl; }
    
    // loop through reconstructed showers.
    for (size_t s=0; s < ev_shower->size(); s++) {

      if (_debug) { std::cout << "new shower" << std::endl; }
      auto const& shr = ev_shower->at(s);

      // grab clusters associated with this shower
      auto const& ass_clus_idx_v = ass_pfpart_clus_v.at( ass_shr_pfpart_v.at(s).at(0) );

      if (_debug) { std::cout << "this shower is associated with " << ass_clus_idx_v.size() << " clusters" << std::endl; }
      
      // create new pfparticle
      larlite::pfpart pfpart_new;
      // create association holder for it
      std::vector<unsigned int> pfpart_ass_holder;

      // get the hit indices assocaited with this reco'd shower
      // on the various planes
      std::vector< std::vector<unsigned int> > shr_hit_ass_idx_v;
      shr_hit_ass_idx_v.resize(3); // one entry per plane
      // vector of larlite clusters associated to the shower (per plane)
      std::vector< larlite::cluster > old_cluster_v;
      old_cluster_v.resize(3);

      for (auto const& clus_idx : ass_clus_idx_v){

	if (_debug) { std::cout << "new cluster" << std::endl; }

	// grab associated cluster
	auto const& old_cluster = ev_cluster->at(clus_idx);
	
	// hits associated with this cluster
	auto const& hit_idx_v = ass_clus_hit_v.at(clus_idx);
	if (_debug) { std::cout << "this cluster is associated with " << hit_idx_v.size() << " hits" << std::endl; }
	// find plane
	size_t pl = ev_hit_shower->at(hit_idx_v.at(0)).WireID().Plane;
	if (_debug) { std::cout << "cluster plane : " << pl << std::endl; }
	// loop over hits and add the indices:
	for (auto const& hit_idx : hit_idx_v)
	  shr_hit_ass_idx_v.at(pl).push_back(hit_idx);

	old_cluster_v.at(pl) = old_cluster;
	
      }// for all clusters associated to shower

      // project shower on the 3 planes
      if (_debug) { std::cout << "projecting shower on planes" << std::endl; }
      projectShower(shr);
      
      // loop over 3 planes
      for (size_t pl=0; pl < 3; pl++) {

	if (_debug) { std::cout << "get shower polygon for plane " << pl << std::endl; }

	auto const& shrPoly = _shr_polygon_v.at(pl);

	if (_debug) { std::cout << "hits on plane before merging : " << shr_hit_ass_idx_v.at(pl).size() << std::endl; }

	if (_debug) { std::cout << "polygon has size : " << shrPoly.Size() << std::endl; }
	
	// if no hits associated on this plane -> skip
	if (shr_hit_ass_idx_v.at(pl).size() == 0) continue;
	
	// loop over photons for this plane
	if (_debug) { std::cout << "loop over photons " << std::endl; }

	for (auto const& photonPoly : _photon_poly_v.at(pl) ) {
	  
	  // check the number of hits in the cluster
	  auto nhits = ass_photon_hit_v.at(photonPoly.first).size();

	  if (_debug) { std::cout << "\n\n\t\t new photon with " << nhits << " hits" << std::endl; }
	  
	  // apply cut on fraction of photon chareg (# of hits here) that can be added to existing shower
	  if (nhits > (shr_hit_ass_idx_v.at(pl).size() * _frac_shr_q) ) continue;

	  // get linearity
	  auto photonLin = _photon_lin_map[ photonPoly.first ];

	  // do the polygons overlap?
	  bool overlap = ( shrPoly.Overlap( photonPoly.second) || shrPoly.Contained( photonPoly.second) );

	  if (overlap == false) {
	    if (_debug) std::cout << "\t no overlap w/ shower cone... continue" << std::endl;
	    continue;
	  }

	  // for large clusters, add extra checks
	  if (nhits > 8) {
	    
	    // apply cut on slope agreement
	    if (slopeCompat( shrPoly, photonLin) > _max_slope_angle) continue;
	    
	    // make sure the photon doesn't extend on both sides of the shower cone
	    if (photonCrossesShower(shrPoly, photonPoly.second) == true) continue;

	  }// if cluster is large

	  // made it this far -> merge the photon with the shower
	  
	  // get set of hits to add (removing potential duplicates)
	  mergeHits( shr_hit_ass_idx_v.at(pl), ass_photon_hit_v.at(photonPoly.first) );
	  if (_debug) { std::cout << "\t\t hits merged. there are " << shr_hit_ass_idx_v.at(pl).size() << " hits." << std::endl; }
	  
	}// compatible showers
	
	// create cluster with newly identified shower hits on this plane
	// new hit indices for this cluster
	auto const& new_hit_idx_v = shr_hit_ass_idx_v.at(pl);
	larlite::cluster shr_clus_new;
	if (_debug) { std::cout << "creating new cluster with " << new_hit_idx_v.size() << " hits" << std::endl; }
	shr_clus_new.set_n_hits( new_hit_idx_v.size() );
	shr_clus_new.set_view( ev_hit_shower->at( new_hit_idx_v.at(0) ).View() );
	// get info from old cluster
	auto const& old_cluster = old_cluster_v.at(pl);
	shr_clus_new.set_start_wire ( old_cluster.StartWire(),  1. );
	shr_clus_new.set_end_wire   ( old_cluster.EndWire(),    1. );
	shr_clus_new.set_start_tick ( old_cluster.StartTick(),  1. );
	shr_clus_new.set_end_tick   ( old_cluster.EndTick(),    1. );
	shr_clus_new.set_start_angle( old_cluster.StartAngle()     );
	shr_clus_new.set_planeID    ( old_cluster.Plane()          );
	std::vector<unsigned int> new_clus_ass_hit_idx_v;
	for (auto const& hit_idx : new_hit_idx_v)
	  new_clus_ass_hit_idx_v.push_back( hit_idx );
	// add
	clus_hit_ass_holder_new.push_back( new_clus_ass_hit_idx_v );
	ev_cluster_new->emplace_back( shr_clus_new );
	pfpart_ass_holder.push_back( ev_cluster_new->size() - 1 );
	if (_debug) { std::cout << "done creating new cluster" << std::endl; }
	
      }// for all 3 planes

      ev_pfpart_new->emplace_back( pfpart_new );
      pfpart_clus_ass_holder_new.push_back( pfpart_ass_holder );
      
    }// for all reconstructed showers

    ev_cluster_hit_ass_v_new   ->set_association( ev_cluster_new->id(), product_id(data::kHit,ev_hit_shower->name()),       clus_hit_ass_holder_new    );
    ev_pfpart_cluster_ass_v_new->set_association( ev_pfpart_new->id(),  product_id(data::kCluster,ev_cluster_new->name()),  pfpart_clus_ass_holder_new );    
    
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

    auto start  = shr.ShowerStart();
    auto dir    = shr.Direction();
    auto oangle = shr.OpeningAngle();
    if (_width > 0) oangle = _width * 3.14 / 180.;
      

    auto const& end = start + _shrLen * dir;

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
      double shrWidth = _shrLen * fabs(tan(oangle));

      // unlike length, width is not stretched or compressed on projection.
      // extend end-point "left" and "right" by one width to complete triangle
      // extend in direction perpendicular to line connecting start_pl and end_pl
      double slope = -1. / ( (end_pl.t - start_pl.t) / (end_pl.w - start_pl.t) );
      if (_debug)
	{
	  std::cout << "\t End pt  = [ "  << end_pl.w << ", " << end_pl.t << " ]" << std::endl;
	  std::cout << "\t SLOPE = " << slope << std::endl;
	  std::cout << "\t WIDTH = " << shrWidth << std::endl;
	}

      // find triangle base coordinate points
      double pt1_w = end_pl.w + sqrt( shrWidth / (1 + slope*slope) );
      double pt1_t = end_pl.t + slope * sqrt( shrWidth / (1 + slope*slope) );
      triangle_coordinates.push_back( std::pair<float,float>( pt1_w, pt1_t) );

      double pt2_w = end_pl.w - sqrt( shrWidth / (1 + slope*slope) );
      double pt2_t = end_pl.t - slope * sqrt( shrWidth / (1 + slope*slope) );
      triangle_coordinates.push_back( std::pair<float,float>( pt2_w, pt2_t) );

      if (_debug){
	std::cout << "SHOWER COORDINATES @ PLANE " << pl <<  " : " << std::endl
		  << "\t Vertex @ [ " << start_pl.w << ", " << start_pl.t << " ]" << std::endl
		  << "\t Pt1    @ [ " << pt1_w      << ", " << pt1_t      << " ]" << std::endl
		  << "\t Pt2    @ [ " << pt2_w      << ", " << pt2_t      << " ]" << std::endl;
      }
      
      twodimtools::Poly2D triangle(triangle_coordinates);
      _shr_polygon_v[pl] = triangle;
      
      
    }// for all 3 planes
    
    return;
  }
  
  double ConeOverlapTag::slopeCompat(const twodimtools::Poly2D& shr,
				     const twodimtools::Linearity& photon) {
    
    
    // get slope of photon
    auto const& photon_slope_val = photon._slope;
    // and error
    auto const& photon_slope_err = photon._slope_err;

    // get shower slope
    double shr_slope_val;

    double ydiff = ( (shr.Point(2).second + shr.Point(1).second) / 2. ) - shr.Point(0).second;
    double xdiff = ( (shr.Point(2).first + shr.Point(1).first) / 2. ) - shr.Point(0).first;

    shr_slope_val = ydiff/xdiff;

    // and error
    double shr_slope_max = (shr.Point(2).second - shr.Point(0).second) / (shr.Point(2).first - shr.Point(0).first);

    double shr_slope_err = fabs( shr_slope_max - shr_slope_val );

    double angle = fabs ( atan( ( shr_slope_val - photon_slope_val ) / ( 1 + photon_slope_val * shr_slope_val ) ) );

    angle *= (180./3.14);

    if (_debug) { std::cout << "\t photon slope angle = " << angle << std::endl; }

    return angle;

    /*
    if (_debug) {
      std::cout << "Shower slope = " << shr_slope_val    << " +/- " << shr_slope_err << std::endl
		<< "Photon slope = " << photon_slope_val << " +/- " << photon_slope_err << " w/ NHits = "  << photon._dof + 2 << std::endl;
    }

    // is the slope difference larger than the sum of the errors? if so not compatible
    if ( fabs( shr_slope_val - photon_slope_val) > (shr_slope_err + photon_slope_err) ) return false;

    if (_debug) { std::cout << "slopes are compatible! " << std::endl; }
    
    return true;
    */
    
  }

  bool ConeOverlapTag::photonCrossesShower(const twodimtools::Poly2D& shr,
					   const twodimtools::Poly2D& photon) {

    // get the triangle points
    double Ox = shr.Point(0).first;
    double Oy = shr.Point(0).second;
    double Ax = shr.Point(1).first;
    double Ay = shr.Point(1).second;
    double Bx = shr.Point(2).first;
    double By = shr.Point(2).second;

    // check where the shower end-point lies
    double Ex = (Ax+Bx)/2.;
    double Ey = (Ay+By)/2.;

    double signEA = (Ex-Ox)*(Ay-Oy) - (Ey-Oy)*(Ax-Ox);
    double signEB = (Ex-Ox)*(By-Oy) - (Ey-Oy)*(Bx-Ox);
    
    // how many photon edges are on the left or right of the cone?
    int slope_left  = 0;
    int slope_right = 0;

    // being on one side or another of the segments OA and OB depends on the sign
    // of the equations used to compute signEA and signEB
    // to check if points are on either side of the cone compare their sign
    // with the signs of EA and EB knowing that these two are in the center.

    for (size_t i=0; i < photon.Size(); i++) {

      auto const& pt = photon.Point(i);

      double signA = (pt.first-Ox)*(Ay-Oy) - (pt.second-Oy)*(Ax-Ox);
      double signB = (pt.first-Ox)*(By-Oy) - (pt.second-Oy)*(Bx-Ox);

      if ( ((signA*signEA) < 0) && ((signB*signEB) > 0) ) slope_left  += 1;
      if ( ((signA*signEA) > 0) && ((signB*signEB) < 0) ) slope_right += 1;

    }// for all photon points

    if ( (slope_left > 0) and (slope_right > 0) ){

      if (_debug) std::cout << "\t photon crosses shower" << std::endl;
      return true;
    }

    return false;
  }
    
  
  void ConeOverlapTag::mergeHits(std::vector<unsigned int>& shr_hit_idx_v,
				 const std::vector<unsigned int>& photon_hit_idx_v) {

    // static copy of shower hit index vector to use for iteration
    std::vector<unsigned int> shr_hit_idx_v_old = shr_hit_idx_v;
    
    for(auto const& photon_hit_idx : photon_hit_idx_v) {
      bool duplicate = false;
      for (auto const& shr_hit_idx : shr_hit_idx_v_old) {
	if (photon_hit_idx == shr_hit_idx) {
	  duplicate = true;
	  continue;
	}// if duplicate
      }// for all shower hits
      if (duplicate == false)
	shr_hit_idx_v.push_back(photon_hit_idx);
    }// for all photon hits

    return;
  }

  std::vector<twodimtools::Poly2D> ConeOverlapTag::getPhotonPolygon(int pl) {

    std::vector<twodimtools::Poly2D> poly_v;

    auto polypair = _photon_poly_v.at(pl);

    for (auto const& poly : polypair)
      poly_v.push_back( poly.second );

    return poly_v;
  }


}
#endif
