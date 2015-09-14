#ifndef ERTOOL_ERANACRTAGGER_CXX
#define ERTOOL_ERANACRTAGGER_CXX

#include "ERAnaCRTagger.h"

namespace ertool {

  ERAnaCRTagger::ERAnaCRTagger(const std::string& name) : AnaBase(name)
    , _event_tree(nullptr)
    , _int_tree(nullptr)
    , _part_tree(nullptr)
    , fTPC(0.,-115.5,0.,254.8,117.5,1036.92)
  {}

  void ERAnaCRTagger::Reset()
  {}

  void ERAnaCRTagger::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaCRTagger::ProcessBegin()
  {
    if ( !_event_tree ){
	_event_tree = new TTree("event_tree","");
    	_event_tree->Branch("run",&_run,"run/i");
    	_event_tree->Branch("subrun",&_subrun,"subrun/i");
    	_event_tree->Branch("event",&_event,"event/i");
    	_event_tree->Branch("ctr_cosmic",&_ctr_cosmic,"ctr_cosmic/i");
    	_event_tree->Branch("ctr_non_cosmic",&_ctr_non_cosmic,"ctr_non_cosmic/i");
    }
    
    if ( !_part_tree ){
	_part_tree = new TTree("part_tree","");
    	_part_tree->Branch("run",&_run,"run/i");
    	_part_tree->Branch("subrun",&_subrun,"subrun/i");
    	_part_tree->Branch("event",&_event,"event/i");
    	_part_tree->Branch("pdg",&_pdg,"pdg/I");
    	_part_tree->Branch("primary",&_primary,"primary/i");
    	_part_tree->Branch("start_x",&_start_x,"start_x/D");
    	_part_tree->Branch("start_y",&_start_y,"start_y/D");
    	_part_tree->Branch("start_z",&_start_z,"start_z/D");
    	_part_tree->Branch("end_x",&_end_x,"end_x/D");
    	_part_tree->Branch("end_y",&_end_y,"end_y/D");
    	_part_tree->Branch("end_z",&_end_z,"end_z/D");
    	_part_tree->Branch("px",&_px,"px/D");
    	_part_tree->Branch("py",&_py,"py/D");
    	_part_tree->Branch("pz",&_pz,"pz/D");
    }

    if ( !_int_tree ){
	_int_tree = new TTree("int_tree","");
    	_int_tree->Branch("int_x",&_int_x,"int_x/D");
    	_int_tree->Branch("int_y",&_int_y,"int_y/D");
    	_int_tree->Branch("int_z",&_int_z,"int_z/D");
    	_int_tree->Branch("primary_pdg",&_primary_pdg,"primary_pdg/D");
    	_int_tree->Branch("ctr_child",&_ctr_child,"ctr_child/D");
    	_int_tree->Branch("ctr_level",&_ctr_level,"ctr_level/D");
    	_int_tree->Branch("angle",&_angle,"angle/D");
    	_int_tree->Branch("length",&_length,"length/D");
    }
  }

  void ERAnaCRTagger::ResetTree(){

    _ctr_cosmic = 0 ;
    _ctr_non_cosmic = 0 ;

    _int_x = -999;
    _int_y = -999;
    _int_z = -999;
    _primary_pdg = -999 ;
    _ctr_child   = -1;
    _ctr_level   = -1;
    _distToTopWall = -99999;
    _distToWall    = -9999;
    _distBackAlongTraj = -9999;
    _angle = -999;
    _length = -999;

   _run = -99;
   _subrun = -99; 
   _event = -99;
   _pdg = -99;;
   _primary = -99;
   _start_x = -999;
   _start_y = -999;
   _start_z = -999;
   _end_x = -999;
   _end_y = -999;
   _end_z = -999;
   _px = -999;
   _py = -999;
   _pz = -999;

    return;
  }

  bool ERAnaCRTagger::Analyze(const EventData &data, const ParticleGraph &graph)
  {

    _run    = data.Run() ;
    _subrun = data.SubRun() ;
    _event  = data.Event_ID() ;
    
    double detHalfHeight = 116.5 ;

    for( auto const& p : graph.GetParticleArray() ){

      _pdg = p.PdgCode() ; 
      _primary = p.Primary() ; 
      _start_x = p.Vertex()[0]; 
      _start_y = p.Vertex()[1];
      _start_z = p.Vertex()[2];
      _end_x = p.Vertex()[0]; 
      _end_y = p.Vertex()[1];
      _end_z = p.Vertex()[2];
      
      _px = p.Momentum()[0];
      _py = p.Momentum()[1];
      _pz = p.Momentum()[2];
      
      _part_tree->Fill(); 
      
    }
    
    //Every primary particle should correspond wiht an 'interaction'
    for( auto const& track : graph.GetPrimaryNodes(RecoType_t::kTrack)){
	 
      ResetTree();
      auto const& t = data.Track(graph.GetParticle(track).RecoID());
      
      if (graph.GetParticle(track).ProcessType() == kCosmic )
	_ctr_cosmic++;
      else
	_ctr_non_cosmic++;
      
      _ctr_child = graph.GetAllDescendantNodes(track).size();
      
      ::geoalgo::HalfLine trk(t.at(0),t.at(0).Dir()); //Start().Position(),t.Start().Momentum());
      
      _int_x = t.at(0)[0] ;
      _int_y = t.at(0)[1] ;
      _int_z = t.at(0)[2] ;
      _primary_pdg = t._pid ;
      _length = t.Length(); 
      CalculateAngleYZ(graph.GetParticle(track),_angle);
      
      if( fTPC.Contain(t.at(0)) && _geoAlgo.Intersection(fTPC,trk,true).size() > 0){
	_distBackAlongTraj = sqrt(_geoAlgo.Intersection(fTPC,trk,true)[0].SqDist(t.at(0))) ;
	_distToTopWall     = (_int_y - detHalfHeight)/t.at(0).Dir()[1] ; // py is normalize, pmag is not--so ignore pmag
	_distToWall        = sqrt(_geoAlgo.SqDist(t.at(0),fTPC));
      }   
      else if ( !fTPC.Contain(t.at(0)) && _geoAlgo.Intersection(fTPC,trk,true).size() > 0){ 
	//There seems to be an issue with setting the new points when x < 0-- they always
	//get set to 0,0,0.  This is a temporary fix
	if( _int_x >= 0 ){
	  _int_x = _geoAlgo.Intersection(fTPC,trk,true).at(0)[0] ;
	  _int_y = _geoAlgo.Intersection(fTPC,trk,true).at(0)[1] ;
	  _int_z = _geoAlgo.Intersection(fTPC,trk,true).at(0)[2] ;
	}   
	else
	  _int_x = 0 ; 
	
	_distToWall = 0 ; 
	_distBackAlongTraj = 0;
	_distToTopWall = (_int_y - detHalfHeight)/t.at(0).Dir()[1] ;
	
	//std::cout<<"If we're here, what are x,y,z: "<<_int_x<<", "<<_int_y<<", "<<_int_z<<std::endl ;
      }
      else
	ResetTree();
      //
      _int_tree->Fill();
    }
    
    _event_tree->Fill();
    
    return true;
  }

  void ERAnaCRTagger::ProcessEnd(TFile* fout)
  {
    if(fout) {
      fout->cd();
      _int_tree->Write();
      _event_tree->Write();
      _part_tree->Write();
    }
    return;
  }

  void ERAnaCRTagger::CalculateAngleYZ ( const Particle & p , double & angle) {
	
      auto px = p.Momentum()[0];
      auto py = p.Momentum()[1];
      auto pz = p.Momentum()[2];
      double quad = py/pz  ;
      double convert = 180.0/ 3.1415926525898 ;

      if(quad < 0)
           quad *=-1;

      angle = atan(quad );
      angle*= convert ;

      auto momMag = sqrt(px*px + py*py + pz*pz);
      auto py_Norm = py / momMag ;
      auto pz_Norm = pz / momMag ;

      if(py_Norm >=0 && pz_Norm <0)
        angle = 180 - angle ;
      else if(py_Norm <0 && pz_Norm <0)
        angle = 180 + angle ;
      else if(py_Norm <0 && pz_Norm >=0)
        angle = 360 - angle ;

	return;
    }
}

#endif
