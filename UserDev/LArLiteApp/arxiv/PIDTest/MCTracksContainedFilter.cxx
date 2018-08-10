#ifndef MCTRACKSCONTAINEDFILTER_CXX
#define MCTRACKSCONTAINEDFILTER_CXX

#include "MCTracksContainedFilter.h"

namespace larlite {
  int countin = 0.;
  int countout = 0.;
  bool MCTracksContainedFilter::initialize() {

    //Set DistToBoxWall's "box" to be TPC 
    _myGeoAABox.Min( 0,
		     -(::larutil::Geometry::GetME()->DetHalfHeight()),
		     0);
    
    _myGeoAABox.Max( 2*(::larutil::Geometry::GetME()->DetHalfWidth()),
		     ::larutil::Geometry::GetME()->DetHalfHeight(),
		     ::larutil::Geometry::GetME()->DetLength());
    
    return true;
  }
  
  bool MCTracksContainedFilter::analyze(storage_manager* storage) {

    //Grab the MCTracks
    auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");    
    if(!ev_mctrack) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctrack!"));
      return false;
    }

    //Make sure all MC tracks are fully contained
    for (auto &track : *ev_mctrack)
      {
	if ( !isFullyContained(track) ) {countout++; return false;}
      }
    //Looks like you want to keep this event.
    countin++;
    return true;

  }

  bool MCTracksContainedFilter::finalize() {
    std::cout<<"Kept Events: "<<countin<<"; Rejected Events: "<<countout<<";"<<std::endl;

    return true;
  }

  bool MCTracksContainedFilter::isFullyContained(mctrack const & mytrack){

    //Make sure track MC start point and MC end point are in active volume

    ::geoalgo::Point_t mystartpoint(mytrack.Start().Position());
    ::geoalgo::Point_t myendpoint(mytrack.End().Position());
    
    if(_myGeoAABox.Contain(mystartpoint) &&
       _myGeoAABox.Contain(myendpoint)) return true;

    return false;
  }
}
#endif
