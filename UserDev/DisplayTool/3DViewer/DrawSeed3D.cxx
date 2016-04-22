#ifndef DRAWSEED3D_CXX
#define DRAWSEED3D_CXX

#include "DrawSeed3D.h"

namespace evd {


DrawSeed3D::DrawSeed3D() {
  _name = "DrawSeed3D";
  _fout = 0;
}

bool DrawSeed3D::initialize() {

  return true;
}

bool DrawSeed3D::analyze(larlite::storage_manager* storage) {

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
  auto seedHandle = storage->get_data<larlite::event_seed>(_producer);

  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(seedHandle -> size());


  // Populate the shower vector:
  for (auto & seed : *seedHandle) {
    Seed3D temp;

    double point[3], direction[3],error[3];


    seed.GetPoint(point,error);
    seed.GetDirection(direction,error);

    temp._point[0] = point[0];
    temp._point[1] = point[1];
    temp._point[2] = point[2];
    temp._direction[0] = direction[0];
    temp._direction[1] = direction[1];
    temp._direction[2] = direction[2];

    temp._length = seed.GetLength();

    _data.push_back(temp);
  }


  return true;
}

bool DrawSeed3D::finalize() {

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

DrawSeed3D::~DrawSeed3D() {}


} // evd

#endif
