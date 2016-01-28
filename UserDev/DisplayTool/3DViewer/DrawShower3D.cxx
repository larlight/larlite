#ifndef DRAWTRACK3D_CXX
#define DRAWTRACK3D_CXX

#include "DrawShower3D.h"
#include "DataFormat/shower.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawShower3D::DrawShower3D() {
  _name = "DrawShower3D";
  _fout = 0;
}

bool DrawShower3D::initialize() {

  return true;
}

bool DrawShower3D::analyze(larlite::storage_manager* storage) {

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





  // get a handle to the tracks
  auto showerHandle = storage->get_data<larlite::event_shower>(_producer);

  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _data.clear();
    _data.reserve(showerHandle -> size());
  }


  // Populate the shower vector:
  for (auto & shower : *showerHandle) {
    _data.push_back(getShower3d(shower));
  }


  return true;
}

bool DrawShower3D::finalize() {

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

DrawShower3D::~DrawShower3D() {}

Shower3D DrawShower3D::getShower3d(larlite::shower shower) {
  Shower3D result;


  result._start_point = shower.ShowerStart();
  result._direction = shower.Direction();
  result._length = shower.Length();
  result._opening_angle = shower.OpeningAngle();

  return result;
}


} // evd

#endif
