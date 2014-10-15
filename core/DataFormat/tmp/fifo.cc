#ifndef FIFO_CC
#define FIFO_CC

#include "fifo.hh"

namespace larlight {

  //***************************************************************************
  fifo::fifo(DATA::DATA_TYPE type) : std::vector<UShort_t>(),
				     data_base(type)
  //***************************************************************************
  {
    
    if(_type!=DATA::FIFO &&
       _type!=DATA::PMTFIFO &&
       _type!=DATA::TPCFIFO) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::FIFO;
    }

    clear_data();
  }

  //***************************************************************************
  void fifo::clear_data()
  //***************************************************************************
  {

    data_base::clear_data();
    clear();
    init_vars();
  }

  //***************************************************************************  
  void fifo::init_vars()
  //***************************************************************************
  {
    _channel_number=FEM::INVALID_CH;
    _module_address=DATA::INVALID_UCHAR;
    _module_id=DATA::INVALID_UCHAR;
    _readout_frame_number=FEM::INVALID_WORD;
    _readout_sample_number=FEM::INVALID_WORD;
  }

  ////////////////////////////////////////////////////////

  //***************************************************************************
  event_fifo::event_fifo(DATA::DATA_TYPE type) : std::vector<larlight::fifo>(), 
						 event_base(type)
  //***************************************************************************
  { 
    if(_type!=DATA::FIFO &&
       _type!=DATA::PMTFIFO &&
       _type!=DATA::TPCFIFO) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::FIFO;
    }    
    clear_data();
  }

  //***************************************************************************  
  void event_fifo::clear_data(){
  //***************************************************************************
    event_base::clear_data();
    clear();
    init_vars();
  }

  //***************************************************************************
  void event_fifo::init_vars(){
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
