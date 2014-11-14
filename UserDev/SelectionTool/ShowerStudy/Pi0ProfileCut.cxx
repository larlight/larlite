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

    _vtx = ::geoalgo::TwoLineIntersection().Intersection3D(a_vtx, b_vtx,
							   a_dir, b_dir);
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
