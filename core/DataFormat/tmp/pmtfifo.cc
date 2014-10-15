#ifndef FIFO_CC
#define FIFO_CC

#include "pmtfifo.hh"

namespace larlight {

  //***************************************************************************
  pmtfifo::pmtfifo() : fifo::fifo(DATA::PMTFIFO)
  //***************************************************************************
  {
    if(_type!=DATA::PMTFIFO) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::PMTFIFO;
    }
    
    clear_data();
  }

  //***************************************************************************
  void pmtfifo::clear_data()
  //***************************************************************************
  {

    data_base::clear_data();
    clear();
    init_vars();
  }

  //***************************************************************************  
  void pmtfifo::init_vars() 
  //***************************************************************************
  {
    fifo::init_vars();
    _disc_id=FEM::DISC_MAX;
  }

  ////////////////////////////////////////////////////////

  //***************************************************************************
  event_pmtfifo::event_pmtfifo(DATA::DATA_TYPE type) : std::vector<larlight::pmtfifo>(), 
				   event_base(type)
  //***************************************************************************
  { 
    clear_data();
  }

  //***************************************************************************  
  void event_pmtfifo::clear_data(){
  //***************************************************************************
    event_base::clear_data();
    clear();
    init_vars();
  }

  //***************************************************************************
  void event_pmtfifo::init_vars(){
  //***************************************************************************
    _event_frame_number=FEM::INVALID_WORD;
    _module_address=DATA::INVALID_UCHAR;
    _module_id=DATA::INVALID_UCHAR;
    _checksum=FEM::INVALID_WORD;
    _nwords=FEM::INVALID_WORD;
    _fem_trig_frame_number=FEM::INVALID_WORD;
    _fem_trig_sample_number=FEM::INVALID_WORD;
  }

}
#endif
