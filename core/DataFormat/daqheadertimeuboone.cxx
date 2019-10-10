////////////////////////////////////////////////////////////////////////
//
// DAQHeaderTimeUBooNE class
//
// Copied & modified from ubobj/RawData/DAQHeaderTimeUbooNE.cxx
//
////////////////////////////////////////////////////////////////////////

#include "daqheadertimeuboone.h"

#ifndef LARLITE_DAQHEADERTIMEUBOONE_CXX
#define LARLITE_DAQHEADERTIMEUBOONE_CXX

namespace larlite{

  //----------------------------------------------------------------------
  // Default constructor.

  daqheadertimeuboone::daqheadertimeuboone() : data_base(data::kDAQHeaderTimeUBooNE){
    fGPSTime=0;
    fNTPTime=0;
    fPPSsec=0;
    fPPSmicro=0;
    fPPSnano=0;
    fTrigFrame=0;
    fTrigSample=0;
    fTrigDiv=0;
    fTrigPPSFrame=0;
    fTrigPPSSample=0;
    fTrigPPSDiv=0;
    fTimeStamp=0;
 
    clear_data();
  }

  void daqheadertimeuboone::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
  }

}
#endif
