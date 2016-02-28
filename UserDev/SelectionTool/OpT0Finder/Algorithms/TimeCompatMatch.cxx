#ifndef OPT0FINDER_TIMECOMPATMATCH_CXX
#define OPT0FINDER_TIMECOMPATMATCH_CXX

#include "TimeCompatMatch.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"
#include <cmath>
#include <sstream>

namespace flashana {

  TimeCompatMatch::TimeCompatMatch(const std::string name)
    : BaseProhibitAlgo(name)
  {
    _frame_drift_time = 2300.; // usec
  }

  void TimeCompatMatch::Configure(const ::fcllite::PSet &pset)
  {
    _frame_drift_time = pset.get<double>("FrameDriftTime");
  }

  bool TimeCompatMatch::MatchCompatible(const QCluster_t& clus, const Flash_t& flash)
  {


    _frame_drift_time = 2319 ;

    // conversion quantities
    double t2cm   = larutil::GeometryUtilities::GetME()->TimeToCm();
    double ROrate = larutil::DetectorProperties::GetME()->SamplingRate(); // ns

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

    double det_width = 256.35; // cm
    bool tpc_bound (true);
    if ( clus_x_min<0. || clus_x_max>det_width ) tpc_bound=false; // we have translated any one of the trk points out of the TPC.
    // convert both quantities to time (usec)
    double clus_t_min = (clus_x_min/t2cm)*(ROrate/1000.); // us
    double clus_t_max = (clus_x_max/t2cm)*(ROrate/1000.); // us

    // find the largest distance in time between the flash
    // and the cluster's time
    // if the cluster's time is more than a drift-window larger
    // then the flash -> impossible coincidence
    //


    // This condition is moved to PhotonLibHypothesis.cxx, since the question must be asked with each step in the 
    // translation of x, which does not happen here: here we only check global track - flash compatibility issues.
    /*
    if ( (fabs(clus_t_max - flash_time) > _frame_drift_time)  
	 //	 || ( (clus_t_min - clus_tdrift_min*1000.) - flash_time > 0 )  
	 //	 ||         !tpc_bound
	 )
      {
	std::cout<<"Failed clus t min, t max, clus_tdrift_min, flash_time, frame_drift_time [all usec]: "<<clus_t_min<<", "<<clus_t_max<<", "<<clus_tdrift_min*1000.<<", "<<flash_time<<", "<<_frame_drift_time<<std::endl;
	return false;
      
    }
    */
    // if the cluster comes before the flash entirely ->
    // impossible match
    if ( (clus_t_min+20.) < flash_time) {
  //    std::cout<<"min failur! "<<clus_t_min+20.<<", "<<flash_time<<", "<<std::endl; 
      return false;
      }
    
    return true;
  }


}
#endif
