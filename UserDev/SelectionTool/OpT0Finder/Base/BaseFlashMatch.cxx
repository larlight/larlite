#ifndef OPT0FINDER_BASEFLASHMATCH_CXX
#define OPT0FINDER_BASEFLASHMATCH_CXX

#include "BaseFlashMatch.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"

namespace flashana {

  Flash_t BaseFlashMatch::GetEstimate(const QCluster_t& tpc) const
  {
    return _flash_hypothesis->GetEstimate(tpc);
  }

  void BaseFlashMatch::FillEstimate(const QCluster_t& tpc, Flash_t& opdet) const
  {
    _flash_hypothesis->FillEstimate(tpc,opdet);
  }

  void BaseFlashMatch::OOBCheck(const QCluster_t& tpc, Flash_t& opdet, const Flash_t& opmeas) const
  {

    double t2cm   = larutil::GeometryHelper::GetME()->TimeToCm();
    double ROrate = larutil::DetectorProperties::GetME()->SamplingRate(); // ns
    bool oob(false);

    double clus_x_min =  1036.; // cm
    double clus_x_max =  -1036.; // cm
    double clus_tdrift_min = 1E9; //nsec, independent of translation of track's x
    const double det_width = 256.35; // cm

    for (auto const& pt : tpc){
      if (pt.x < clus_x_min) { clus_x_min = pt.x; }
      if (pt.x > clus_x_max) { clus_x_max = pt.x; }
      if (pt.t < clus_tdrift_min) { clus_tdrift_min = pt.t; }
    }
    // Remember this is the hypothesized flash. We just instantiated it in function (Q)LLMatch::ChargeHypothesis that called
    // this one. So, the time is some unphysical default value. If I want code to be generalizable to non-beam flashes
    // then I need measured flash.time, and can't take it to be ~0.

    // The earliest part of the track can not arrive after the upper bound measured drift time if it is to belong to this flash.
    double clus_t_min = (clus_x_min/t2cm)*(ROrate/1000.); // us
    if ( ( clus_t_min - clus_tdrift_min/1000. ) >  opmeas.time
	 ||  clus_x_max > det_width
	 ||  clus_x_min < 0.0
	 )  // us
      { oob = true; }

    // zero the flash if the track was translated till it stepped out of TPC
    if (oob)
      for ( auto& v : opdet.pe_v ) v = 0;


  }


  void BaseFlashMatch::SetFlashHypothesis(flashana::BaseFlashHypothesis* alg)
  {
    _flash_hypothesis = alg;
  }

}

#endif
