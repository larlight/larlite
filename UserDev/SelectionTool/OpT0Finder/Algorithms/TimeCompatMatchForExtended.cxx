#ifndef OPT0FINDER_TIMECOMPATMATCH_FOREXTENDED_CXX
#define OPT0FINDER_TIMECOMPATMATCH_FOREXTENDED_CXX

#include "TimeCompatMatchForExtended.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "LArUtil/LArProperties.h"
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

    const float cm_per_tick = ::larutil::LArProperties::GetME()->DriftVelocity()*0.5;
    
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

    // we assume positions are relative to the trigger. so is the flash time!

    // calculate TPC tick range
    //double clus_t_min = clus_x_min/DriftVelocity();
    double clus_t_min = (clus_x_max - ActiveXMax())/DriftVelocity();
    double clus_t_max = clus_x_min/DriftVelocity();

    if ( clus_t_min > clus_t_max ) {
      // why would this happen
      if ( _verbosity<2 )
	std::cout << "Had to switch min/max" << std::endl;
      double temp = clus_t_min;
      clus_t_min = clus_t_max;
      clus_t_max = temp;
    }


    if ( _verbosity<2 ) {
      std::cout<< "Inspecting TPC object [" << clus.idx << "] @ " << clus.time << std::endl;
      std::cout<< "xmin = " << clus_x_min << " ... xmax = " << clus_x_max << std::endl;
      std::cout<< "tmin = " << clus_t_min << " ... tmax = " << clus_t_max << std::endl;
      std::cout<< "Flash[" << flash.idx << "] time @ " << flash_time << std::endl;
    }
    return ((clus_t_min - _time_buffer) < flash_time && flash_time < (clus_t_max + _time_buffer));

  }


}
#endif
