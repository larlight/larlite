
#ifndef LARLITE_RAEDIGIT_CXX
#define LARLITE_RAEDIGIT_CXX

#include "rawdigit.h"

namespace larlite {

  //--------------------------------------------------
  short rawdigit::ADC(int i) const
  {
    unsigned int j = i;
    if(i < 0 || (j>fADC.size()) ) {
      std::cerr << "\033[93m" << __PRETTY_FUNCTION__ <<"\033[00m"
		<< "asked for illegal ADC index: " << i << "\n";
      throw std::exception();
    }

    return fADC[j];
  }


  //----------------------------------------------------------------------
  void rawdigit::SetPedestal(double ped)
  {

    fPedestal = ped;
    fSigma = 1.;

  }

}
#endif
