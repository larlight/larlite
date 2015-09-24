#ifndef DRAWTRACK_CXX
#define DRAWTRACK_CXX

#include "DrawTrack.h"
#include "DataFormat/track.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawTrack::DrawTrack() {
  _name = "DrawTrack";
  _fout = 0;
}

bool DrawTrack::initialize() {

  // Resize data holder
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;
}

bool DrawTrack::analyze(larlite::storage_manager* storage) {

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

  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(trackHandle -> size());
  }


  // Populate the track vector:
  for (auto & track : *trackHandle) {
    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      _dataByPlane.at(view).push_back(getTrack2d(track, view));
    }
  }


  return true;
}

bool DrawTrack::finalize() {

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

DrawTrack::~DrawTrack(){}

Track2d DrawTrack::getTrack2d(larlite::track track, unsigned int plane) {
  Track2d result;
  for (unsigned int i = 0; i < track.NumberTrajectoryPoints(); i++) {
    // project a point into 2D:
    try {
      auto point = geoHelper->Point_3Dto2D(track.LocationAtPoint(i), plane);
      result._track.push_back(std::make_pair(point.w, point.t));
    }
    catch (...) {
      continue;
    }

  }

  return result;
}


} // larlite

#endif
