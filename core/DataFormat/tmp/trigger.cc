#ifndef TRIGGER_CC
#define TRIGGER_CC

#include "trigger.hh"

namespace larlight {

  trigger::trigger(DATA::DATA_TYPE type) : event_base(type)
  {
    if(_type!=DATA::Trigger) {

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));

      _type=DATA::Trigger;
    }

    clear_data();
  }
  
  void trigger::clear_data(){

    event_base::clear_data();
    init_vars();
    
  }
  
  trigger::trigger(const trigger &original) : 
    event_base(original),
    _trig_sample_number(original._trig_sample_number),
    _trig_frame_number(original._trig_frame_number),
    _trig_number(original._trig_number),
    _pmt_data(original._pmt_data),
    _trig_pc(original._trig_pc),
    _trig_ext(original._trig_ext),
    _active(original._active),
    _gate1_in(original._gate1_in),
    _gate2_in(original._gate2_in),
    _veto_in(original._veto_in),
    _calib(original._calib),
    _remainder_64MHz(original._remainder_64MHz),
    _remainder_16MHz(original._remainder_16MHz)
  {};
  
  void trigger::init_vars(){
    
    _trig_sample_number = 0xffff;
    _trig_frame_number  = FEM::INVALID_WORD;
    _trig_number        = FEM::INVALID_WORD;
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
