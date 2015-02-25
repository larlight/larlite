#ifndef MCSHOWERTAGGER_CXX
#define MCSHOWERTAGGER_CXX

#include "MCShowerTagger.h"
#include "DataFormat/mcshower.h"
#include <time.h>

namespace larlite {

  bool MCShowerTagger::initialize() {

	_tag_tree = 0 ;
	PrepareTTree();
	
    _evtN = 0;
	_numberOfEvents = new TH1D("numberOfEvents","Some variable",100,0,200);
	_numberOfShowers = new TH1D("numberOfShowers","variable",100,0,200);

    return true;
  }

void MCShowerTagger::PrepareTTree() {
    
    if(!_tag_tree) {
      _tag_tree = new TTree("tag_tree","");

      _tag_tree->Branch("_run",&_run,"run/I");
      _tag_tree->Branch("_subrun",&_subrun,"subrun/I");
      _tag_tree->Branch("_event",&_event,"event/I");

      _tag_tree->Branch("_X",&_X,"X/D");
      _tag_tree->Branch("_Y",&_Y,"Y/D");
      _tag_tree->Branch("_Z",&_Z,"Z/D");

      _tag_tree->Branch("_Px",&_Px,"Px/D");
      _tag_tree->Branch("_Py",&_Py,"Py/D");
      _tag_tree->Branch("_Pz",&_Pz,"Pz/D");
      _tag_tree->Branch("_E",&_E,"E/D");
	  _tag_tree->Branch("_PDG",&_PDG,"PDG/D");
	  _tag_tree->Branch("_process","std::string",&_process);

   ////PARENT INFO
      _tag_tree->Branch("_parentPDG",&_parentPDG,"parentPDG/I");//---Mother PDG code
      _tag_tree->Branch("_parentX",&_parentX,"parentX/D");//---------G4 start X of mother
      _tag_tree->Branch("_parentY",&_parentY,"parentY/D");//---------G4 start Y of mother
      _tag_tree->Branch("_parentZ",&_parentZ,"parentZ/D");//---------G4 start Z of mother
      _tag_tree->Branch("_parentT",&_parentT,"parentT/D");//---------G4 start T of mother

      _tag_tree->Branch("_parentPx",&_parentPx,"parentPx/D");//------G4 start Px of mother
      _tag_tree->Branch("_parentPy",&_parentPy,"parentPy/D");//------G4 start Py of mother
      _tag_tree->Branch("_parentPz",&_parentPz,"parentPz/D");//------G4 start Pz of mother
      _tag_tree->Branch("_parentE",&_parentE,"parentE/D");//---------G4 start E of mother

      _tag_tree->Branch("_showerScore",&_showerScore,"showerScore/D");
      _tag_tree->Branch("_trackScore",&_trackScore,"trackScore/D");
	  _tag_tree->Branch("_minMuIP",&_minMuIP,"minMuIP/D");
      _tag_tree->Branch("_minMuDist",&_minMuDist,"minMuDist/D");
      _tag_tree->Branch("_distToTopWall",&_distToTopWall,"distToTopWall/D");
      _tag_tree->Branch("_inFV",&_inFV,"inFV/B");
      _tag_tree->Branch("_containedE",&_containedE,"containedE/D");



	}
}

void MCShowerTagger::Reset(){

   _run     = -1;
   _subrun  = -1;
   _event   = -1;
   _PDG	    = -1;
   _process ="NONE";

   _X       = -9999999;
   _Y       = -9999999;
   _Z       = -9999999;

   _Px      = -9999999;
   _Py      = -9999999;
   _Pz      = -9999999;
   _E       = -9999999;

  //parent
   _parentPDG = -1;
   _parentX   = -9999999;
   _parentY   = -9999999;
   _parentZ   = -9999999;
   _parentT   = -9999999;

   _parentPx  = -9999999;
   _parentPy  = -9999999;
   _parentPz  = -9999999;
   _parentE   = -9999999;

	_minMuDist = -999999;
	_minMuIP = -999999;

   _showerScore = -999;
   _trackScore = -999;
   _inFV 	   = -1 ;
   _containedE = -999;

}

  bool MCShowerTagger::analyze(storage_manager* storage) {

	_numberOfEvents->Fill(larutil::Geometry::GetME()->DetHalfHeight() );

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
    
    //make vector of all mc tracks. Do this only once
    _allMCTracks.clear();
    for (size_t m=0; m < evt_mctracks->size(); m++){
      //time-selection (only if in-frame)
      if( (evt_mctracks->at(m).Start().T() > -0.8E6) and (evt_mctracks->at(m).Start().T() < 0.8E6) ){
		 if( evt_mctracks->at(m).size() > 1) 
		   	addTrack(evt_mctracks->at(m));
         }
       }
	if(evt_mctracks->size() ==0){std::cout<<"FOUND IT"<<std::endl; }
//	   std::cout<<"\n\nAll MCtracks: "<<_allMCTracks.size()<<std::endl; 

    _allRecoTracks.clear();
 //   for (size_t m=0; m < evt_tracks->size(); m++){
 //     //time-selection (only if in-frame)
 //     //if( (evt_tracks->at(m).vertex_at(0).T() > -0.8E6) and (evt_tracks->at(m).Start().T() < 0.8E6) ){
 //   	 if( evt_tracks->at(m).n_points() > 1) 
 //   	   addTrack(evt_tracks->at(m));
 //       // }
 //     }


	geoalgo::AABox tpcBox(0,-1*larutil::Geometry::GetME()->DetHalfHeight(),0,
						 2*larutil::Geometry::GetME()->DetHalfWidth(),
						 larutil::Geometry::GetME()->DetHalfHeight(),
						 larutil::Geometry::GetME()->DetLength() );
	int i=0; 
    for (size_t s=0; s < evt_mcshower->size(); s++){

		_numberOfShowers->Fill(larutil::Geometry::GetME()->DetHalfHeight() );

  	  	mcshower shr = evt_mcshower->at(s);
		if(shr.DetProfile().X() <= 0 || shr.DetProfile().X() >= 256.35 || shr.DetProfile().Y() <=-116.5 || shr.DetProfile().Y() >=116.5 || shr.DetProfile().Y() ==0 || shr.DetProfile().Z() <=0 || shr.DetProfile().Z() >= 1037. ) continue;

		Reset() ;
  	 	ass.clear();
		i++; 
  	   

	   _process = shr.Process();
	   _PDG = shr.PdgCode() ;
	   
	   _X = shr.DetProfile().X();
	   _Y = shr.DetProfile().Y();
	   _Z = shr.DetProfile().Z();
	   _E = shr.DetProfile().E();
	   _Px = shr.DetProfile().Px();
	   _Py = shr.DetProfile().Py();
	   _Pz = shr.DetProfile().Pz();


	   _containedE = _E/shr.Start().E() ;


  	if(_PDG == 22){
        _parentPDG = shr.PdgCode();
        _parentX   = shr.Start().X();
        _parentY   = shr.Start().Y();
        _parentZ   = shr.Start().Z();
        _parentT   = shr.Start().T();
        _parentPx  = shr.Start().Px();
        _parentPy  = shr.Start().Py();
        _parentPz  = shr.Start().Pz();
        _parentE   = shr.Start().E();
      }   
      // otherwise -> electron/positron's mother is the "mother"
     else{
          _parentPDG = shr.MotherPdgCode();
          _parentX = shr.MotherStart().X();
          _parentY = shr.MotherStart().Y();
          _parentZ = shr.MotherStart().Z();
          _parentT = shr.MotherStart().T();
          _parentPx = shr.MotherStart().Px();
          _parentPy = shr.MotherStart().Py();
          _parentPz = shr.MotherStart().Pz();
          _parentE = shr.MotherStart().E();
        }   
   
	  if(_Px == 0 && _Py==0 && _Pz ==0)
	  	continue;

	if(_X >25 && _X < 231 && _Y >-91 && _Y<91 && _Z >30 && _Z <936)
		_inFV =1 ;
	else
		_inFV =0;


  	   geoalgo::HalfLine shower(_X,_Y,_Z,_Px,_Py,_Pz);
	   _distToTopWall  = (shower.Start()[1] - 116)*shower.Dir().Length()/(shower.Dir()[1]) ;

  	   float cosmicScoreShrBox, cosmicScoreShrTrk;

  	   cosmicScoreShrTrk = _tagger.ShowerTrackScore(_allMCTracks,shower);	
	   cosmicScoreShrBox = _tagger.ShowerBoxScore(shower, tpcBox) ;

	   _trackScore = cosmicScoreShrTrk ;
	   _showerScore = cosmicScoreShrBox ;

	   //Adding cosmic tag for showers with respect to all tracks
 // 	   cosmictag thistag0(cosmicScoreShrTrk);
 // 	   shower_tag->push_back(thistag0);
 // 	   ass.push_back(shower_tag->size()-1);
 // 	   mcshr_to_cosmictag.push_back(ass);



	   //Adding cosmic tag for showers with respect to box 
  //	   cosmictag thistag1(cosmicScoreShrBox);
  //	   shower_tag->push_back(thistag1);
  //	   ass.push_back(shower_tag->size()-1);
  //	   mcshr_to_cosmictag.push_back(ass);

//	  if(thistag0.fCosmicScore != 0){
	  // std::cout<<"\nCyl dist: "<<minDist <<std::endl;
	  // std::cout<<"ShowerID: "<<shr.TrackID()<<std::endl;
	  // std::cout<<"First tag: "<<thistag0.fCosmicScore <<std::endl;
	  // std::cout<<"Second tag: "<<thistag1.fCosmicScore <<std::endl;
//	   }
		_tag_tree->Fill() ;

  	   }//for all showers 
//	   std::cout<<"Showers that passed the volume cut: "<<i<<std::endl;
   
//    evt_mcshower->set_association(shower_tag->id(),mcshr_to_cosmictag);
 
    _evtN += 1;
    
    return true;
  }

  bool MCShowerTagger::finalize() {

   if(_fout) {
    	_fout->cd();
  		_numberOfEvents->Write() ;
  		_numberOfShowers->Write() ;
        if(_tag_tree)
        	_tag_tree->Write();
     }
   
     else
       print(larlite::msg::kERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
   
     delete _tag_tree;

    return true;
  }


  void MCShowerTagger::addTrack(const mctrack& track){

    geoalgo::Trajectory_t thisTrack(0,3);    
    
    for (size_t i=0; i < track.size(); i++)
       thisTrack.push_back( {track.at(i).X(), track.at(i).Y(), track.at(i).Z()} );

//	if(thisTrack.size() == 0) { std::cout<<"thisTrack's size: "<<thisTrack.size()<<std::endl ; }

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
