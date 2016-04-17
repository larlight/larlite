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
    _vtx.clear();
    
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
  
  void PiZeroROI::SetVertex( const std::vector< std::pair < int, int > > vtx){
    
    if(vtx.size() > 3){
      try{ 
	throw vtx.size();
      }
      catch(int e){
	std::cout << "Too many vertices, " << e << std::endl; 
      }
    }
    
    _vtx = vtx;
    
  }
  
  std::vector < std::pair <int, int > > PiZeroROI::GetVertex() const { return _vtx; }
    
  std::vector < std::pair< int, int > > PiZeroROI::GetWireROI() const { return _wire_range; }
  
  std::vector < std::pair< int, int > > PiZeroROI::GetTimeROI() const { return  _t_range; }

  std::vector < std::pair< int, int > > PiZeroROI::GetPiZeroWireROI() const{ return _pi0_wire_range; }
  
  std::vector < std::pair< int, int > > PiZeroROI::GetPiZeroTimeROI() const { return _pi0_t_range; }
  
  
  
}

#endif
