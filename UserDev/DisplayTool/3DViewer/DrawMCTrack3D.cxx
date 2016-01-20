#ifndef DRAWMCMCTRACK3D_CXX
#define DRAWMCTRACK3D_CXX

#include "DrawMCTrack3D.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawMCTrack3D::DrawMCTrack3D() {
  _name = "DrawMCTrack3D";
  _fout = 0;
}

bool DrawMCTrack3D::initialize() {

  // Resize data holder
  // if (_data.size() != geoService -> Nviews()) {
  //   _data.resize(geoService -> Nviews());
  // }
  return true;
}

bool DrawMCTrack3D::analyze(larlite::storage_manager* storage) {

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
  auto mctrackHandle = storage->get_data<larlite::event_mctrack>(_producer);

  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _data.clear();
    _data.reserve(mctrackHandle -> size());
  }


  // Populate the track vector:
  for (auto & mctrack : *mctrackHandle) {
      _data.push_back(getMCTrack3d(mctrack));
  }
  
  return true;
}

bool DrawMCTrack3D::finalize() {

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

DrawMCTrack3D::~DrawMCTrack3D(){}

MCTrack3D DrawMCTrack3D::getMCTrack3d(larlite::mctrack mctrack) {
  MCTrack3D result;
  for (size_t i = 0; i < mctrack.size(); i++) {
    // project a point into 2D:
    try {
      result._track.push_back(mctrack.at(i).Position().Vect());
    }
    catch (...) {
      continue;
    }

  }

  return result;
}


} // evd

#endif
