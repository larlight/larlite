#ifndef BASICTOOL_GEOALGOMPLBACKEND_CXX
#define BASICTOOL_GEOALGOMPLBACKEND_CXX

#include "GeoAlgoMPLBackEnd.h"

namespace mpl {

  XYZ_v GeoAlgoMPLBackEnd::Convert(const geoalgo::Trajectory_t &data)
  {
    XYZ_v res(3);
    if(!data.size()) 
      throw ::geoalgo::GeoAlgoException("<<Convert>> Input trajectory is empty!");
    if(data[0].size() != 2 && data[0].size() !=3)
      throw ::geoalgo::GeoAlgoException("<<Convert>> Only 2D or 3D object supported!");

    res[0].reserve(data.size());
    res[1].reserve(data.size());
    if(data[0].size()==3) res[2].reserve(data.size());

    for(auto const& pt : data) {

      for(size_t i=0; i<pt.size(); ++i)

	res[i].push_back(pt[i]);
    }      
    
    return res;
  }
  
  XYZ_v GeoAlgoMPLBackEnd::Convert(const geoalgo::AABox_t &data)
  {
    return XYZ_v();

  }
  
  XYZ_v GeoAlgoMPLBackEnd::Convert(const geoalgo::LineSegment_t &data)
  { return XYZ_v(); }


}

#endif
