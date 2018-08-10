#ifndef PI0PROFILECUT_CXX
#define PI0PROFILECUT_CXX

#include "Pi0ProfileCut.h"

namespace showerana {

  Pi0ProfileCut::Pi0ProfileCut()
  {
    Reset();
  }

  void Pi0ProfileCut::SetDaughterInfo( std::vector<double> const& a_vtx,
				       std::vector<double> const& b_vtx,
				       std::vector<double> const& a_dir,
				       std::vector<double> const& b_dir )
  {
    if( a_vtx.size() < 3 ||
	b_vtx.size() < 3 ||
	a_dir.size() < 3 ||
	b_dir.size() < 3 ) 

      throw ShowerAnaException("Input vtx/dir size must be >=3!");

    for(size_t i=0; i<3; ++i) {

      _a_vtx[i] = a_vtx[i];
      _b_vtx[i] = b_vtx[i];
      _a_dir[i] = a_dir[i];
      _b_dir[i] = b_dir[i];

    }


    ///Make _vtx be the midpoint of the line representing the closest approach of two infinite lines
    // Kaleko wrote this and hasn't tested it yet.
    // Using constructor that takes in x0, y0, z0, x1, y1, z1
    geoalgo::Line_t line1(a_vtx[0],a_vtx[1],a_vtx[2],a_vtx[0]+a_dir[0],a_vtx[1]+a_dir[1],a_vtx[2]+a_dir[2]);
    geoalgo::Line_t line2(b_vtx[0],b_vtx[1],b_vtx[2],b_vtx[0]+b_dir[0],b_vtx[1]+b_dir[1],b_vtx[2]+b_dir[2]);
    //These points will be filled w/ points on each line corresponding
    //to the point of closest approach w/ the other line
    geoalgo::Point_t pt1(3);
    geoalgo::Point_t pt2(3);
    _myGeoAlgo.SqDist(line1,line2,pt1,pt2);
    _vtx = (pt2+pt1)/2;

  }

  void Pi0ProfileCut::Reset() {

    _vtx.clear();	
    _a_vtx.clear();
    _b_vtx.clear();
    _a_dir.clear();
    _b_dir.clear();
    
    _vtx.resize   (4, std::numeric_limits<double>::max());
    _a_vtx.resize (3, std::numeric_limits<double>::max());
    _b_vtx.resize (3, std::numeric_limits<double>::max());
    _a_dir.resize (3, std::numeric_limits<double>::max());
    _b_dir.resize (3, std::numeric_limits<double>::max());
    
  }

}

#endif
