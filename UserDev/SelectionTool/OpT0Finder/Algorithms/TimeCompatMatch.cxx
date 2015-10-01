#ifndef OPT0FINDER_TIMECOMPATMATCH_CXX
#define OPT0FINDER_TIMECOMPATMATCH_CXX

#include "TimeCompatMatch.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"
#include <cmath>
#include <sstream>

namespace flashana {

  TimeCompatMatch::TimeCompatMatch()
  {
    _frame_drift_time = 1600; // usec
  };

  bool TimeCompatMatch::MatchCompatible(const QCluster_t& clus, const Flash_t& flash)
  {
    // Prepare the return
    FlashMatch_t f;
    QCluster_t flash_hypothesis_v;
    
    // conversion quantities
    double t2cm   = larutil::GeometryUtilities::GetME()->TimeToCm();
    double ROrate = larutil::DetectorProperties::GetME()->SamplingRate(); // ns

    if(clus.empty()) return false;

    // get time of flash
    auto flash_time = flash.time;

    // get time of cluster by looking at the range of x-positions
    double clus_x_min = 1036.; // cm
    double clus_x_max = 0.;    // cm
    for (auto const& pt : clus){
      if (pt.x > clus_x_max) { clus_x_max = pt.x; }
      if (pt.x < clus_x_min) { clus_x_min = pt.x; }
    }

    // convert both quantities to time (usec)
    double clus_t_min = (clus_x_min/t2cm)*(ROrate/1000.); // us
    double clus_t_max = (clus_x_max/t2cm)*(ROrate/1000.); // us
    
    // find the largest distance in time between the flash
    // and the cluster's time
    // if either greater than the total drift time -> no match possible
    if ( fabs( clus_t_min - flash_time ) > _frame_drift_time )
      return false;
    if ( fabs( clus_t_max - flash_time ) > _frame_drift_time )
      return false;
    
    return true;
  }


}
#endif
