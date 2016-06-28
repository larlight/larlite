#ifndef PMT_FIFO_CXX
#define PMT_FIFO_CXX

#include "pmtfifo.h"

namespace larlite {

  //***************************************************************************
  //  pmtfifo::pmtfifo() : fifo::fifo(data::kPMTFIFO)
  pmtfifo::pmtfifo() : fifo::fifo()
  //***************************************************************************
  {

    // if(_type!=data::kPMTFIFO) {
      
    //   Message::send(msg::kERROR,__FUNCTION__,
    // 		    Form("Provided data type (%s) not supported! Reset to default.",DATA::DATA_TREE_NAME[_type].c_str()));
    //   _type=data::kPMTFIFO;
    // }
    
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
    _disc_id=fem::kDISC_MAX;
  }

  ////////////////////////////////////////////////////////

  //***************************************************************************
  // event_pmtfifo::event_pmtfifo(data::DataType_t type) : std::vector<larlite::pmtfifo>(), 
  //   event_base(type)
  // //***************************************************************************
  // { 
  //   clear_data();
  // }

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
