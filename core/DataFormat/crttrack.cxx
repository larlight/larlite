#ifndef LARLITE_CRTTRACK_CXX
#define LARLITE_CRTTRACK_CXX

#include "crttrack.h"

namespace larlite {

  //#################################################
  crttrack::crttrack() : data_base(data::kCRTTrack)
				 //#################################################
  {

    feb_id.clear();
    pesmap.clear();
    peshit=0;
     
    ts0_s=0;
    ts0_s_err=0;
      
    ts0_ns=0;
    ts0_ns_err=0;

    ts1_ns=0;
    ts1_ns_err=0;
      
    plane1=-1;
    plane2=-1;
    x1_pos=-1;
    x1_err=-1;
    y1_pos=-1;
    y1_err=-1;
    z1_pos=-1;
    z1_err=-1;
    x2_pos=-1;
    x2_err=-1;
    y2_pos=-1;
    y2_err=-1;
    z2_pos=-1;
    z2_err=-1;
    length=-1;
    thetaxy=-1;
    phiz=-1;

    ts0_ns_h1=0;
    ts0_ns_err_h1=0;

    ts0_ns_h2=0;
    ts0_ns_err_h2=0;

    ts2_ns_h1=0;
    ts2_ns_h2=0;
    
    clear_data();
  }

  //##########################################################################
  void crttrack::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
  }
  
}
#endif
//nothing to do here
