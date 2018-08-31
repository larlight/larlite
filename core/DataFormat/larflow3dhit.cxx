#ifndef LARLITE_LARFLOW3DHIT_CXX
#define LARLITE_LARFLOW3DHIT_CXX

#include "larflow3dhit.h"

namespace larlite {

  //#################################################
  larflow3dhit::larflow3dhit() : data_base(data::kLArFlow3DHit)
  //#################################################
  {
    clear_data();
  }

  //##########################################################################
  void larflow3dhit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
  }
  
}
#endif
  
