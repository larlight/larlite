#ifndef LARLITE_PIZEROROI_CXX
#define LARLITE_PIZEROROI_CXX

#include "PiZeroROI.h"
#include <iostream>

namespace larlite{

  void PiZeroROI::clear_data(){
    _t_range.clear();
    _wire_range.clear();
    _pi0_wire_range.clear();
    _pi0_t_range.clear();
    _muon_vtx.clear();
    _nu_vtx.clear();
    _vtx.clear();
    _trk_end.clear();
  }
  
  PiZeroROI::PiZeroROI(const std::vector < std::pair< int, int > > Wire,
		       const std::vector < std::pair< int, int > > Time){
    
    if( Wire.size() != Time.size()){
      try{
	throw Wire.size();
      }
      catch(int e){
	std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    
      }
      return;
    }
    
    if( Wire.size() > 3 || Time.size() > 3){
      std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
      return;    
    }
    
    _wire_range = Wire;
    _t_range = Time;
    
  }

  //Overloaded Initializer
  PiZeroROI::PiZeroROI(const std::vector < std::pair< int, int > > Wire,
		       const std::vector < std::pair< int, int > > Time, 
		       const std::vector < std::pair< int, int > > PiZeroWire,
		       const std::vector < std::pair< int, int > > PiZeroTime){
  
  if( Wire.size() != Time.size()){
    try{
      throw Wire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }

  if( PiZeroWire.size() != PiZeroTime.size()){
    try{
      throw PiZeroWire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }


  if( Wire.size() > 3 || Time.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }

  if( PiZeroWire.size() > 3 || PiZeroTime.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }

  
  _wire_range = Wire;
  _t_range = Time;
  _pi0_wire_range = PiZeroWire;
  _pi0_t_range = PiZeroTime;

}
  
  // Setter Function Definitions 
  void PiZeroROI::SetROI(const std::vector < std::pair< int, int > > Wire,
			 const std::vector < std::pair< int, int > > Time){
    
    if( Wire.size() != Time.size()){
      try{
	throw Wire.size();
      }
      catch(int e){
	std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
      }
      return;
    }
    
    if( Wire.size() > 3 || Time.size() > 3){
      std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
      return;    
    }
    
    _wire_range = Wire;
    _t_range = Time;
    
  }

//Overloaded Setter
  void PiZeroROI::SetROI(const std::vector < std::pair< int, int > > Wire,
			 const std::vector < std::pair< int, int > > Time,
			 const std::vector < std::pair< int, int > > PiZeroWire,
			 const std::vector < std::pair< int, int > > PiZeroTime){
    
    if( Wire.size() != Time.size()){
      try{
	throw Wire.size();
      }
      catch(int e){
	std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
      }
      return;
    }
    
    if( PiZeroWire.size() != PiZeroTime.size()){
      try{
	throw PiZeroWire.size();
      }
      catch(int e){
	std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
      return;
    }
    
    
    if( Wire.size() > 3 || Time.size() > 3){
      std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
      return;    
    }
    
    if( PiZeroWire.size() > 3 || PiZeroTime.size() > 3){
      std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
      return;    
    }
    
    _wire_range = Wire;
    _t_range = Time;
    _pi0_wire_range = PiZeroWire;
    _pi0_t_range = PiZeroTime;
  }
  
  void PiZeroROI::SetPiZeroROI(const std::vector < std::pair< int, int > > PiZeroWire,
			       const std::vector < std::pair< int, int > > PiZeroTime){
    
    if( PiZeroWire.size() != PiZeroTime.size()){
      try{
	throw PiZeroWire.size();
      }
      catch(int e){
	std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
      }
      return;
    }
    
    
    if( PiZeroWire.size() > 3 || PiZeroTime.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
    }
    
    
    _pi0_wire_range = PiZeroWire;
    _pi0_t_range = PiZeroTime;
  }
  
  void PiZeroROI::SetTrackEnd( const std::vector< std::pair < int, int > > trk_end){
    
    if(trk_end.size() > 3){
      try{ 
	throw trk_end.size();
      }
      catch(int e){
	std::cout << "Too many track endneseses, " << e << std::endl; 
      }
    }
    
    _trk_end = trk_end;
    
  }

  void PiZeroROI::SetMuonVertex( const std::vector<float> vtx){
    
    _muon_vtx = vtx;
    
  }

  void PiZeroROI::SetNeutrinoVertex( const std::vector<float> vtx){
    
    _nu_vtx = vtx;
    
  }

  void PiZeroROI::SetVertex( const std::vector< std::pair < int, int > > vtx){
    
    if(vtx.size() > 3){
      try{ 
	throw vtx.size();
      }
      catch(int e){
	std::cout << "Too many vertexes, " << e << std::endl; 
      }
    }
    
    _vtx = vtx;
    
  }

  
  std::vector<float> PiZeroROI::GetMuonVertex() const { return _muon_vtx; }
  std::vector<float> PiZeroROI::GetNeutrinoVertex() const { return _nu_vtx; }

  std::vector < std::pair <int, int > > PiZeroROI::GetVertex() const { return _vtx; }

  std::vector < std::pair <int, int > > PiZeroROI::GetTrackEnd() const { return _trk_end; }
    
  std::vector < std::pair< int, int > > PiZeroROI::GetWireROI() const { return _wire_range; }
  
  std::vector < std::pair< int, int > > PiZeroROI::GetTimeROI() const { return  _t_range; }

  std::vector < std::pair< int, int > > PiZeroROI::GetPiZeroWireROI() const{ return _pi0_wire_range; }
  
  std::vector < std::pair< int, int > > PiZeroROI::GetPiZeroTimeROI() const { return _pi0_t_range; }
  
  
  
}

#endif
