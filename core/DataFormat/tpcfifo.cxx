#ifndef FIFO_CXX
#define FIFO_CXX

#include "tpcfifo.h"

namespace larlite {

  //***************************************************************************
  tpcfifo::tpcfifo() : fifo::fifo()
  //***************************************************************************
  {

    // if(_type!=DATA::TPCFIFO) {
      
    //   Message::send(MSG::ERROR,__FUNCTION__,
    // 		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
    //   _type=DATA::TPCFIFO;
    // }

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
    _plane=larlite::geo::kUnknown;
    _signal=larlite::geo::kMysteryType;
  }

  ////////////////////////////////////////////////////////

  //***************************************************************************
  event_tpcfifo::event_tpcfifo(data::DataType_t type) : std::vector<larlite::tpcfifo>(), 
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
    _event_frame_number=fem::kINVALID_WORD;
    _module_address=data::kINVALID_UCHAR;
    _module_id=data::kINVALID_UCHAR;
    _checksum=fem::kINVALID_WORD;
    _nwords=fem::kINVALID_WORD;
    _fem_trig_frame_number=fem::kINVALID_WORD;
    _fem_trig_sample_number=fem::kINVALID_WORD;
  }

}
#endif
