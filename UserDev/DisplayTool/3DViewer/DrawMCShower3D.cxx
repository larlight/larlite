#ifndef DRAWMCSHOWER3D_CXX
#define DRAWMCSHOWER3D_CXX

#include "DrawMCShower3D.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawMCShower3D::DrawMCShower3D() {
  _name = "DrawMCShower3D";
  _fout = 0;
}

bool DrawMCShower3D::initialize() {

  // Resize data holder
  // if (_data.size() != geoService -> Nviews()) {
  //   _data.resize(geoService -> Nviews());
  // }
  return true;
}

bool DrawMCShower3D::analyze(larlite::storage_manager* storage) {

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
  auto mcshowerHandle = storage->get_data<larlite::event_mcshower>(_producer);

  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(mcshowerHandle -> size());


  // Populate the track vector:
  for (auto & mcshower : *mcshowerHandle) {
    _data.push_back(getMCShower3D(mcshower));
  }

  return true;
}

bool DrawMCShower3D::finalize() {

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

DrawMCShower3D::~DrawMCShower3D() {}

MCShower3D DrawMCShower3D::getMCShower3D(larlite::mcshower mcshower) {
  MCShower3D result;

  result._start_point = mcshower.Start().Position().Vect();
  result._direction = mcshower.StartDir();
  result._pdg = mcshower.PdgCode();
  float energy = mcshower.Start().E();
  result._length = 13.8874 + 0.121734 * energy - (3.75571e-05) * energy * energy;;
  result._opening_angle = 15 * atan(10.1 / energy);

  return result;

}


} // evd

#endif