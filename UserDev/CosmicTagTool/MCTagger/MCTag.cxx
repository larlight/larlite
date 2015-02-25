#ifndef MCTAG_CXX
#define MCTAG_CXX

#include "MCTag.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/cosmictag.h"

namespace larlite {

  bool MCTag::initialize() {

    return true;
  }
  
  bool MCTag::analyze(storage_manager* storage) {

    // get MCShowers
    auto evt_mcshower = storage->get_data<event_mcshower>(_dataType);
    // get MCTracks
    auto evt_mctracks = storage->get_data<event_mctrack>(_dataType);  
    // create new cosmictag for tracks
    auto track_tag    = storage->get_data<event_cosmictag>("mctrackmctag");
    // now for showers
    auto shower_tag   = storage->get_data<event_cosmictag>("mcshowermctag");

    // Associations:
    AssSet_t mcshr_to_cosmictag;
    mcshr_to_cosmictag.reserve(evt_mcshower->size());
    AssSet_t mctrk_to_cosmictag;
    mctrk_to_cosmictag.reserve(evt_mctracks->size());

    // Single Association Holder
    AssUnit_t ass;
    
    //----------------------
    // First tag the tracks.
    // Look at simb::Origin_t object
    for (size_t t=0; t < evt_mctracks->size(); t++){

      ass.clear();
      
      if (evt_mctracks->at(t).Origin() == simb::Origin_t::kCosmicRay){
	// Tag as cosmic
	cosmictag thistag(1);
	track_tag->push_back(thistag);
	ass.push_back(track_tag->size()-1);
	
      }
      else{
	// Tag as not cosmic
	cosmictag thistag(0);
	track_tag->push_back(thistag);
	ass.push_back(track_tag->size()-1);
      }

      mctrk_to_cosmictag.push_back(ass);
    }// for all tracks

    // Finally, finalize the association
    track_tag->set_association(evt_mctracks->id(),mctrk_to_cosmictag);

    //------------------------
    // Do the same for Showers
    // If shower's ancestor was a cosmic track, then tag as cosmic
    for (size_t s=0; s < evt_mcshower->size(); s++){
      
      ass.clear();
      int cosmic = 0;

      // Given one shower, loop over all tracks
      for (size_t t=0; t < evt_mctracks->size(); t++){

	// Do shower's ancestor & track's TrackIDs match?
	if ( evt_mcshower->at(s).AncestorTrackID() == evt_mctracks->at(t).TrackID() ){
	  // Found ancestor. Is it cosmic?
	  if ( evt_mctracks->at(t).Origin() == simb::Origin_t::kCosmicRay )
	    cosmic = 1;
	}// if track is shower's ancestor

      }// for all tracks
	
      cosmictag thistag(cosmic);
      shower_tag->push_back(thistag);
      ass.push_back(shower_tag->size()-1);
      
      mcshr_to_cosmictag.push_back(ass);
    }// For all showers

    shower_tag->set_association(evt_mcshower->id(),mcshr_to_cosmictag);

    return true;
    }

  bool MCTag::finalize() {

  
    return true;
  }

}
#endif
