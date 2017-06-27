#ifndef LARLITE_ROI_CXX
#define LARLITE_ROI_CXX

#include "roi.h"
#include <iostream>

#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"

namespace larlite{

  roi::roi() : data_base(data::kROI)
  { clear_data(); }

  void roi::clear_data(){
    _time_range.clear();
    _wire_range.clear();
    _vtx.clear();
  }
  
  roi::roi(const std::vector < std::pair< int, int > > Wire,
	   const std::vector < std::pair< int, int > > Time){
    
    if( Wire.size() != Time.size()){
      try{
	throw Wire.size();
      }
      catch(int e){
	std::cout << "roi vector sizes do not match, size "<< e << std::endl;
      }
      return;
    }
    
    if( Wire.size() > 3 || Time.size() > 3){
      std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
      return;    
    }
    
    _wire_range = Wire;
    _time_range = Time;
    
  }

  roi::roi(const larlite::vertex& vtx, const double& radius) {

    _wire_range.resize(3);
    _time_range.resize(3);
    _vtx.resize(3);
    
    std::vector<double> xyz = {vtx.X(), vtx.Y(), vtx.Z()};

    double wire2cm  = larutil::GeometryHelper::GetME()->WireToCm();
    double time2cm  = larutil::GeometryHelper::GetME()->TimeToCm();

    auto geoH = larutil::GeometryHelper::GetME();
    auto geom = larutil::Geometry::GetME();
    for (size_t pl = 0; pl < 3; pl++){

      // get the time-coordinate of the wires on this plane
      double *origin;
      origin = new double[3];
      geom->PlaneOriginVtx(pl,origin);

      auto const& pt = geoH->Point_3Dto2D(xyz,pl);

      double vtxwcm = pt.w;
      double vtxtcm = pt.t;
      
      vtxtcm += 800 * time2cm - origin[0];

      int wmin = (int) ( (vtxwcm - radius) / wire2cm );
      int wmax = (int) ( (vtxwcm + radius) / wire2cm );
      
      int tmin = (int) ( (vtxtcm - radius) / time2cm );
      int tmax = (int) ( (vtxtcm + radius) / time2cm );

      _wire_range[pl] = std::make_pair( wmin, wmax );
      _time_range[pl] = std::make_pair( tmin, tmax );

      _vtx[pl]        = std::make_pair( vtxwcm / wire2cm , vtxtcm / time2cm );
      
    }// for all 3 planes      

  }

  // Setter Function Definitions 
  void roi::SetROI(const std::vector < std::pair< int, int > > Wire,
		   const std::vector < std::pair< int, int > > Time){
    
    if( Wire.size() != Time.size()){
      try{
	throw Wire.size();
      }
      catch(int e){
	std::cout << "roi vector sizes do not match, size "<< e << std::endl;
      }
      return;
    }
    
    if( Wire.size() > 3 || Time.size() > 3){
      std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
      return;    
    }
    
    _wire_range = Wire;
    _time_range = Time;
    
  }

  void roi::SetVertex( const std::vector< std::pair < int, int > > vtx){
    
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
  
  
  const std::vector < std::pair <int, int > >& roi::GetVertex() const { return _vtx; }
  
  const std::vector < std::pair< int, int > >& roi::GetWireROI() const { return _wire_range; }
  
  const std::vector < std::pair< int, int > >& roi::GetTimeROI() const { return  _time_range; }

  event_roi::event_roi(std::string name) : event_base(data::kROI,name) { clear_data(); }

}

#endif
