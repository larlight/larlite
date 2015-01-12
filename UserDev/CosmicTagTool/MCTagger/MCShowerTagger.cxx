#ifndef MCSHOWERTAGGER_CXX
#define MCSHOWERTAGGER_CXX

#include "MCShowerTagger.h"
#include <time.h>

namespace larlite {

  bool MCShowerTagger::initialize() {

    _evtN = 0;

    return true;
  }

  bool MCShowerTagger::analyze(storage_manager* storage) {

    // get MCShowers, Tracks
    auto evt_mcshower = storage->get_data<event_mcshower>("mcreco");
    auto evt_mctracks = storage->get_data<event_mctrack>("mcreco");

    // create new cosmictag for showers 
    auto track_tag    = storage->get_data<event_cosmictag>("mctrackmctag");
    auto shower_tag   = storage->get_data<event_cosmictag>("mcshowermctag");

    // Associations:
    AssSet_t mcshr_to_cosmictag;
    mcshr_to_cosmictag.reserve(evt_mcshower->size());
    AssSet_t mctrk_to_cosmictag;
    mctrk_to_cosmictag.reserve(evt_mctracks->size());

    // Single Association Holder
    AssUnit_t ass;
    
    //make vector of all tracks. Do this only once
    _allMCTracks.clear();
    for (size_t m=0; m < evt_mctracks->size(); m++){
      //time-selection (only if in-frame)
      if( (evt_mctracks->at(m).Start().T() > -0.8E6) and (evt_mctracks->at(m).Start().T() < 0.8E6) ){
		 if( evt_mctracks->at(m).size() > 1) 
		   addTrack(evt_mctracks->at(m));
         }
      }


	geoalgo::AABox tpcBox(0,-1*larutil::Geometry::GetME()->DetHalfHeight(),0,
						 2*larutil::Geometry::GetME()->DetHalfWidth(),
						 larutil::Geometry::GetME()->DetHalfHeight(),
						 larutil::Geometry::GetME()->DetLength() );


    for (size_t s=0; s < evt_mcshower->size(); s++){

  	  	mcshower shr = evt_mcshower->at(s);
  	 	ass.clear();
  	   
  	   _Px = shr.Start().X();
  	   _Py = shr.Start().Y();
  	   _Pz = shr.Start().Z();
  	   _X = shr.DetProfile().X();
  	   _Y = shr.DetProfile().Y();
  	   _Z = shr.DetProfile().Z();
  	   
  	   geoalgo::HalfLine shower(_X,_Y,_Z,_Px,_Py,_Pz);

  	   float cosmicScoreShrBox, cosmicScoreShrTrk;

	   cosmicScoreShrBox = _tagger.ShowerBoxScore(shower, tpcBox) ;
  	   cosmicScoreShrTrk = _tagger.ShowerTrackScore(_allMCTracks,shower);	

  	   cosmictag thistag0(cosmicScoreShrTrk);
  	   shower_tag->push_back(thistag0);
  	   ass.push_back(shower_tag->size()-1);
  	   mcshr_to_cosmictag.push_back(ass);

  	   cosmictag thistag1(cosmicScoreShrBox);
  	   shower_tag->push_back(thistag1);
  	   ass.push_back(shower_tag->size()-1);
  	   mcshr_to_cosmictag.push_back(ass);

  	   }//for all showers 
   
    evt_mcshower->set_association(shower_tag->id(),mcshr_to_cosmictag);
 
    _evtN += 1;
    
    return true;
  }

  bool MCShowerTagger::finalize() {
    

    return true;
  }


  void MCShowerTagger::addTrack(const mctrack& track){

    geoalgo::Trajectory_t thisTrack(0,3);    
    
    for (size_t i=0; i < track.size(); i++)
       thisTrack.push_back( {track.at(i).X(), track.at(i).Y(), track.at(i).Z()} );

   	_allMCTracks.push_back(thisTrack);

  }

  void MCShowerTagger::addTrack(const track& track){

    geoalgo::Trajectory_t thisTrack(0,3);    
    
    for (size_t i=0; i < track.n_point(); i++)
      	thisTrack.push_back( {track.vertex_at(i).X(), track.vertex_at(i).Y(), track.vertex_at(i).Z()} );

    _allRecoTracks.push_back(thisTrack);
  }

}


#endif
