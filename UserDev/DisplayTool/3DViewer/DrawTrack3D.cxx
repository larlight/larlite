#ifndef DRAWTRACK3D_CXX
#define DRAWTRACK3D_CXX

#include "DrawTrack3D.h"
#include "DataFormat/track.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawTrack3D::DrawTrack3D() {
  _name = "DrawTrack3D";
  _fout = 0;
}

bool DrawTrack3D::initialize() {

  // Resize data holder
  // if (_data.size() != geoService -> Nviews()) {
  //   _data.resize(geoService -> Nviews());
  // }
  return true;
}

bool DrawTrack3D::analyze(larlite::storage_manager* storage) {

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
  auto trackHandle = storage->get_data<larlite::event_track>(_producer);

  // Clear out the data but reserve some space 
  _data.clear();
  _data.reserve(trackHandle -> size());


  // Populate the track vector:
  for (auto & track : *trackHandle) {
    _data.push_back(getTrack3d(track));
  }


  return true;
}

bool DrawTrack3D::finalize() {

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

DrawTrack3D::~DrawTrack3D() {}

Track3D DrawTrack3D::getTrack3d(larlite::track track) {
  Track3D result;
  for (size_t i = 0; i < track.NumberTrajectoryPoints(); i++) {
    // project a point into 2D:
    try {
      result._track.push_back(track.LocationAtPoint(i));
    }
    catch (...) {
      continue;
    }

  }

  return result;
}


} // evd

#endif
