#ifndef FIFO_CC
#define FIFO_CC

#include "tpcfifo.hh"

namespace larlight {

  //***************************************************************************
  tpcfifo::tpcfifo() : fifo::fifo(DATA::TPCFIFO)
  //***************************************************************************
  {

    if(_type!=DATA::TPCFIFO) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
      _type=DATA::TPCFIFO;
    }

    clear_data();
  }

  //***************************************************************************
  void tpcfifo::clear_data()
  //***************************************************************************
  {

    data_base::clear_data();
    clear();
    init_vars();
  }

  //***************************************************************************  
  void tpcfifo::init_vars()
  //***************************************************************************
  {
    fifo::init_vars();
    _plane=larlight::GEO::kUnknown;
    _signal=larlight::GEO::kMysteryType;
  }

  ////////////////////////////////////////////////////////

  //***************************************************************************
  event_tpcfifo::event_tpcfifo(DATA::DATA_TYPE type) : std::vector<larlight::tpcfifo>(), 
						 event_base(type)
  //***************************************************************************
  { 
    clear_data();
  }

  //***************************************************************************  
  void event_tpcfifo::clear_data(){
  //***************************************************************************
    event_base::clear_data();
    clear();
    init_vars();
  }

  //***************************************************************************
  void event_tpcfifo::init_vars(){
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
