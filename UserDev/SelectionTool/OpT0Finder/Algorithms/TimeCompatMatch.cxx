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


    _frame_drift_time = 2400 ;

    // conversion quantities
    double t2cm   = larutil::GeometryUtilities::GetME()->TimeToCm();
    double ROrate = larutil::DetectorProperties::GetME()->SamplingRate(); // ns

    if(clus.empty()) {std::cout<<"This is happening"<<std::endl; return false; }

    // get time of flash
    auto flash_time = flash.time;

    // get time of cluster by looking at the range of x-positions
    double clus_x_min =  1036.; // cm
    double clus_x_max = -1036.;    // cm
    for (auto const& pt : clus){
      if (pt.x > clus_x_max) { clus_x_max = pt.x; }
      if (pt.x < clus_x_min) { clus_x_min = pt.x; }
    }
//    std::cout<<"\nclus x min, max: "<<clus_x_min<<", "<<clus_x_max<<", "<<flash_time<<std::endl;

    // convert both quantities to time (usec)
    double clus_t_min = (clus_x_min/t2cm)*(ROrate/1000.); // us
    double clus_t_max = (clus_x_max/t2cm)*(ROrate/1000.); // us

//    std::cout<<"clus t min, max: "<<clus_t_min<<", "<<clus_t_max<<", "<<flash_time<<std::endl;
    
    // find the largest distance in time between the flash
    // and the cluster's time
    // if the cluster's time is more than a drift-window larger
    // then the flash -> impossible coincidence
    if ( fabs(clus_t_max - flash_time) > _frame_drift_time ){
      std::cout<<"Failed clus t min, max: "<<clus_t_min<<", "<<clus_t_max<<", "<<flash_time<<", "<<_frame_drift_time<<std::endl;
      return false;
      }
    // if the cluster comes before the flash entirely ->
    // impossible match
    if ( (clus_t_min+20.) < flash_time) {
	double sum = 0;
	for (int i =0; i < 32 ; i ++ )
	    sum+=flash.pe_v[i] ;

      std::cout<<"min failur! "<<clus_t_min+5.<<", "<<flash_time<<", "<<sum<<std::endl; 

      return false;
      }
    
    return true;
  }


}
#endif
