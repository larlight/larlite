#ifndef LARLITE_TRIGGERDATA_CXX
#define LARLITE_TRIGGERDATA_CXX

#include "trigger.h"

namespace larlite {

  //****************************************************
  bool trigger::Triggered(const unsigned char bit) const
  //****************************************************
  {

    if(bit>32) {
      throw std::invalid_argument("\n\nCannot access bit higher than 32!\n");
      std::exit(1);
    }

    return ( (fTriggerBits >> bit) & 0x1);

  }

  void trigger::clear_data(){

    event_base::clear_data();
    init_vars();
    
  }
  
  void trigger::init_vars(){

    fTriggerNumber       = data::kINVALID_UINT;
	
    fTriggerTime         = data::kINVALID_DOUBLE;
    fBeamGateTime        = data::kINVALID_DOUBLE;
	
    fTriggerBits         = 0x0;

    _trig_sample_number = 0xffff;
    _trig_frame_number  = fem::kINVALID_WORD;
    //_trig_number        = fem::kINVALID_WORD;
    _pmt_data       = 0xffff;
    _trig_pc        = false;
    _trig_ext       = false;
    _active         = false;
    _gate1_in       = false;
    _gate2_in       = false;
    _veto_in        = false;
    _calib          = false;
    _remainder_16MHz = 0xffff;
    _remainder_64MHz = 0xffff;
  }

  
}

#endif
