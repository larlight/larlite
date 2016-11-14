#ifndef OPT0FINDER_TIMECOMPATMATCH_CXX
#define OPT0FINDER_TIMECOMPATMATCH_CXX

#include "TimeCompatMatch.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include <cmath>
#include <sstream>

namespace flashana {

  static TimeCompatMatchFactory __global_TimeCompatMatchFactory__;

  TimeCompatMatch::TimeCompatMatch(const std::string name)
    : BaseProhibitAlgo(name)
  {}

  void TimeCompatMatch::_Configure_(const Config_t &pset)
  {
    _time_buffer = pset.get<double>("TimeBuffer");
  }

  bool TimeCompatMatch::MatchCompatible(const QCluster_t& clus, const Flash_t& flash)
  {

    if(clus.empty()) return false; 

    // get time of flash
    auto flash_time = flash.time;

    // get time of cluster by looking at the range of x-positions
    double clus_x_min =  1036.; // cm
    double clus_x_max = -1036.;    // cm
    double clus_tdrift_min = 1E9; //nsec, independent of translation of track's x
    for (auto const& pt : clus){
      if (pt.x > clus_x_max) { clus_x_max = pt.x; }
      if (pt.x < clus_x_min) { clus_x_min = pt.x; }
      if (pt.t < clus_tdrift_min) { clus_tdrift_min = pt.t; }
    }

    // Earliest flash time => assume clus_x_max is @ detector X-max boundary
    double clus_t_min = (clus_x_max - ActiveXMax()) / DriftVelocity();
    double clus_t_max = clus_x_min / DriftVelocity();

    /*
    std::cout<< "Inspecting TPC object @ " << clus.time << std::endl;
    std::cout<< "xmin = " << clus_x_min << " ... xmax = " << clus_x_max << std::endl;
    std::cout<< "tmin = " << clus_t_min << " ... tmax = " << clus_t_max << std::endl;
    std::cout<< "Flash time @ " << flash_time << std::endl;
    */    
    return ((clus_t_min - _time_buffer) < flash_time && flash_time < (clus_t_max + _time_buffer));

  }


}
#endif
