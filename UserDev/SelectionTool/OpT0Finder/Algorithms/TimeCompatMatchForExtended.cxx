#ifndef OPT0FINDER_TIMECOMPATMATCH_FOREXTENDED_CXX
#define OPT0FINDER_TIMECOMPATMATCH_FOREXTENDED_CXX

#include "TimeCompatMatchForExtended.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include <cmath>
#include <sstream>

namespace flashana {

  static TimeCompatMatchForExtendedFactory __global_TimeCompatMatchForExtendedFactory__;

  TimeCompatMatchForExtended::TimeCompatMatchForExtended(const std::string name)
    : BaseProhibitAlgo(name)
  {}

  void TimeCompatMatchForExtended::_Configure_(const Config_t &pset)
  {
    _time_buffer = pset.get<double>("TimeBuffer");
    _verbosity   = pset.get<int>("Verbosity",2);
  }

  bool TimeCompatMatchForExtended::MatchCompatible(const QCluster_t& clus, const Flash_t& flash)
  {

    if(clus.empty()) return false; 

    // get time of flash
    auto flash_time = flash.time;

    // get time of cluster by looking at the range of x-positions
    // we know there will be extensions
    // so we only consider x-positions where (y,z) are within the detector
    double clus_x_min =  1036.; // cm
    double clus_x_max = -1036.;    // cm
    for (auto const& pt : clus){
      if ( pt.y<ActiveYMin() || pt.y>ActiveYMax() || pt.z<ActiveZMin() || pt.z>ActiveZMax() )
	continue;
      if (pt.x > clus_x_max) { clus_x_max = pt.x; }
      if (pt.x < clus_x_min) { clus_x_min = pt.x; }
    }

    // calculate time between 
    
    // Earliest flash time => assume clus_x_max is @ detector X-max boundary
    double clus_t_min = (clus_x_max - ActiveXMax()) / DriftVelocity();
    double clus_t_max = clus_x_min / DriftVelocity();


    if ( _verbosity<2 ) {
      std::cout<< "Inspecting TPC object @ " << clus.time << std::endl;
      std::cout<< "xmin = " << clus_x_min << " ... xmax = " << clus_x_max << std::endl;
      std::cout<< "tmin = " << clus_t_min << " ... tmax = " << clus_t_max << std::endl;
      std::cout<< "Flash time @ " << flash_time << std::endl;
    }
    return ((clus_t_min - _time_buffer) < flash_time && flash_time < (clus_t_max + _time_buffer));

  }


}
#endif
