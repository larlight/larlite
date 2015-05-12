#ifndef FIFO_CXX
#define FIFO_CXX

#include "fifo.h"

namespace larlite {

  //***************************************************************************
  fifo::fifo() : std::vector<UShort_t>(),
		 data_base(data::kFifo)
  //***************************************************************************
  {
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
    _channel_number=fem::kINVALID_CH;
    _module_address=data::kINVALID_UCHAR;
    _module_id=data::kINVALID_UCHAR;
    _readout_frame_number=fem::kINVALID_WORD;
    _readout_sample_number=fem::kINVALID_WORD;
  }

  ////////////////////////////////////////////////////////

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
