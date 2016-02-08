#ifndef DRAWOPFLASH_CXX
#define DRAWOPFLASH_CXX

#include "DrawOpflash3D.h"

namespace evd {


DrawOpflash3D::DrawOpflash3D() {
  _name = "DrawOpflash3D";
  _fout = 0;
}

bool DrawOpflash3D::initialize() {

  return true;
}

bool DrawOpflash3D::analyze(larlite::storage_manager* storage) {

  //
  // Do your event-by-event analysis here. This function is called for
  // each event in the loop. You have "storage" pointer which contains
  // event-wise data. To see what is available, check the "Manual.pdf":
  //
  // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
  //
  // Or you can refer to Base/DataFormatConstants.hh for available data type
  // enum values. Here is one example of getting PMT waveform collection.
  //
  // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
  //
  // if( event_fifo )
  //
  //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
  //





  // get a handle to the flashes
  auto opflashHandle = storage->get_data<larlite::event_opflash>(_producer);

  _data.clear();
  _data.reserve(opflashHandle -> size());


  // Populate the shower vector:
  for (auto & opf : *opflashHandle) {
    Opflash3D _temp;
    _temp._y = opf.YCenter();
    _temp._z = opf.ZCenter();
    _temp._time = opf.Time();
    _temp._y_width = opf.YWidth();
    _temp._z_width = opf.ZWidth();
    _temp._time_width = opf.TimeWidth();
    _data.push_back(_temp);
  }


  return true;
}

bool DrawOpflash3D::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.
  //
  // Say you made a histogram pointer h1 to store. You can do this:
  //
  // if(_fout) { _fout->cd(); h1->Write(); }
  //
  // else
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
  //
  return true;
}

DrawOpflash3D::~DrawOpflash3D() {}


} // evd

#endif
