#ifndef MCSHOWERTAGGER_CXX
#define MCSHOWERTAGGER_CXX

#include "MCShowerTagger.h"
#include <time.h>

namespace larlite {

  bool MCShowerTagger::initialize() {

    /// count events
    _evtN = 0;

    /// setup _cutParamCalculator (set up TPC boundaries)
    _cutParamCalculator.SetAlgoProperties();

    /// prepare total muon length histogram
    _hTrackTotLen = new TH1D("hTrackTotLen","Summed Length of All Tracks in one Event; Sum length [meters]", 100, 0, 1000);
    _hTrackPDG = new TH1I("hTrackPDG","PDG of All Tracks; PDG Code", 6001, -3000, 3000);
    _hNumTracks = new TH1I("hNumTracks","Number of Tracks / Event; Tracks", 100, 0, 100);

    return true;
  }


  
  bool MCShowerTagger::analyze(storage_manager* storage) {

    auto evt_mcshower = storage->get_data<event_mcshower>("mcreco");
    auto evt_mctracks = storage->get_data<event_mctrack>("mcreco");

    // create new cosmictag for tracks/ showers
    //auto track_tag    = storage->get_data<event_cosmictag>("mctrackmctag");
    auto shower_tag   = storage->get_data<event_cosmictag>("mcshowermctag");

    // Associations
    AssSet_t mcshr_to_cosmictag;
    mcshr_to_cosmictag.reserve(evt_mcshower->size());
    AssSet_t mctrk_to_cosmictag;
    mctrk_to_cosmictag.reserve(evt_mctracks->size());

    // Single Association Holder
    AssUnit_t ass;

    //keep track of total lenght of all muon tracks in event
    double totMuonLen = 0;
    // make a vector of all tracks. Do this only once
    _allTracks.clear();
    _allTrackIDs.clear();
    int nTracks = 0;
    for (size_t m=0; m < evt_mctracks->size(); m++){
      //time-selection (only if in-frame)
      if ( (evt_mctracks->at(m).Start().T() > -0.8E6) and
	   (evt_mctracks->at(m).Start().T() < 0.8E6) ){
	int pdg = evt_mctracks->at(m).PdgCode();
	if( ( (abs(pdg) == 11) or (abs(pdg) == 13) or
	      (abs(pdg == 211)) or (pdg == 2212)
	      or (abs(pdg) == 13) ) and (evt_mctracks->at(m).size() > 1) ){
	  nTracks += 1;
	  totMuonLen += addTrack(evt_mctracks->at(m));
	  _hTrackPDG->Fill(pdg);

	}//if right pdg
      }//if track in-time with frame
    }
    _hNumTracks->Fill(nTracks);
    //    _hTrackTotLen->Fill(totMuonLen/100.);
    // now loop over all showers


	int count=0;
    for (size_t s=0; s < evt_mcshower->size(); s++){
	count++;

      //get current shower
      mcshower shr = evt_mcshower->at(s);

      //Make sure shower is PDG == 11 and mother == ancestor
      if ( 1== 1) {
	  
	_trackID = shr.TrackID();
	_inActiveVolume = 1;
	
	_Px = shr.Start().X();
	_Py = shr.Start().Y();
	_Pz = shr.Start().Z();
	_X = shr.DetProfile().X();
	_Y = shr.DetProfile().Y();
	_Z = shr.DetProfile().Z();
	
	geoalgo::Point_t shrStart(_X, _Y, _Z);
	
	if(_cutParamCalculator.isInVolume(shrStart) && !( _X==0 && _Y==0 && _Z==0 )){
	  _inActiveVolume = 1;
	  
	  double shrMom = sqrt(_Px*_Px+_Py*_Py+_Pz*_Pz);
	  geoalgo::Vector_t shrDir(_Px/shrMom,_Py/shrMom,_Pz/shrMom);
	  
	    // get results from algorithms
	    _cutParamCalculator.getNearestMuonParams(shrStart, shrDir, _allTracks, _allTrackIDs, 0,  _minMuDist, _minMuIP, _distToIP);
//	    _cutParamCalculator.getNearestMuonParams(shrStart, shrDir, _allTracks, _allTrackIDs, shr.AncestorTrackID(), 
//						     _minMuDistExceptAncestor, _minMuIPExceptAncestor, _distToIPExceptAncestor);
//	    _cutParamCalculator.getDistanceToWall(shrStart, shrDir, _distAlongTraj, _distBackAlongTraj);
//	    _hTrackTotLen->Fill(_distBackAlongTraj);

     	float cosmicScore = 0;
      		ass.clear();

//		std::cout<<"Min dist to muon parameter: "<<_minMuDist
//				<<"\nX,Y,Z: "<<_X<<" "<<_Y<<" "<<_Z
//				<<"\nPx,Py,Pz: "<<_Px<<" "<<_Py<<" "<<_Pz <<std::endl;

		//ahack-- get association for first algorithm (min dist to muon).  
		//score by dividing by 100(get a dist between 0 and 1) and assuming 
		//linear relation for now.  Then 1-result => higher score is better.  
		//Put this into function eventually
		if(_minMuDist < 100 ){
			cosmicScore = 1 - ( _minMuDist / 100) ;
     	 	cosmictag thistag(cosmicScore);
      	 	shower_tag->push_back(thistag);
      		ass.push_back(shower_tag->size()-1);
			std::cout<<"minMuDist : "<<cosmicScore<<std::endl; 
		   }
		else{
			cosmicScore = 0; 
			cosmictag thistag(cosmicScore);
			shower_tag->push_back(thistag);
			ass.push_back(shower_tag->size()-1);
			std::cout<<"minMuDist : "<<cosmicScore<<std::endl; 
			}
	    
		//ahack-- get association for second algorithm (IP)
		//For now, assume linear relation again
		if( _minMuIP < 100){
			cosmicScore = 1 - (_minMuIP / 100) ;
			cosmictag thistag(cosmicScore);
			shower_tag->push_back(thistag);
			ass.push_back(shower_tag->size()-1);
			std::cout<<"minIP     : "<<cosmicScore<<std::endl; 
			}
		else{
			cosmicScore = 0 ;
			cosmictag thistag(cosmicScore);
			shower_tag->push_back(thistag);
			ass.push_back(shower_tag->size()-1);
			std::cout<<"minIP     : "<<cosmicScore<<std::endl; 
			}

      		mcshr_to_cosmictag.push_back(ass);

	  }
	  else
	    _inActiveVolume = 0;

	}//if correct shower type
      
    }//for all particles
	std::cout<<"Nuber mcshower is: "<<count<<std::endl;
   
    evt_mcshower->set_association(shower_tag->id(),mcshr_to_cosmictag);
 
    _evtN += 1;
    
    return true;
  }

  bool MCShowerTagger::finalize() {
    
    _hTrackTotLen->Write();
    _hNumTracks->Write();
    _hTrackPDG->Write();

    return true;
  }


  double MCShowerTagger::addTrack(const mctrack& track){

    double totLen = 0;
    geoalgo::Trajectory_t thisTrack(0,3);    
    
    for (size_t i=0; i < track.size(); i++){
      thisTrack.push_back( {track.at(i).X(), track.at(i).Y(), track.at(i).Z()} );
      if (i > 0)
	totLen += pow ( (track.at(i-1).X()-track.at(i).X())*(track.at(i-1).X()-track.at(i).X()) +
			(track.at(i-1).Y()-track.at(i).Y())*(track.at(i-1).Y()-track.at(i).Y()) +
			(track.at(i-1).Z()-track.at(i).Z())*(track.at(i-1).Z()-track.at(i).Z()), 0.5);
    }// for all track steps
    _allTrackIDs.push_back(track.TrackID());
    _allTracks.push_back(thisTrack);
    return totLen;
  }


}
#endif
