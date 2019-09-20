#ifndef LARLITE_CRTHIT_CXX
#define LARLITE_CRTHIT_CXX

#include "crthit.h"

namespace larlite {

  //#################################################
  crthit::crthit() : data_base(data::kCRTHit)
				 //#################################################
  {
    feb_id.clear();
    pesmap.clear();

    peshit=0;
     
    ts0_s=0;
    ts0_s_corr=0;
      
    ts0_ns=0;
    ts0_ns_corr=0;

    ts1_ns=0;

    ts2_ns=0;

    plane=-1;
    x_pos=-1;
    x_err=-1;
    y_pos=-1;
    y_err=-1;
    z_pos=-1;
    z_err=-1;
 
    
    clear_data();
  }

  //##########################################################################
  void crthit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
  }
  
}
#endif
//nothing to do here
