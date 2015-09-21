#ifndef DRAWHIT_CXX
#define DRAWHIT_CXX

#include "DrawHit.h"
#include "DataFormat/hit.h"

namespace evd {


DrawHit::DrawHit() {
  _name = "DrawHit";
  _fout = 0;
}

bool DrawHit::initialize() {
  // // Resize data holder to accommodate planes and wires:
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
    _maxCharge.resize(geoService -> Nviews());
  }
  return true;
}

bool DrawHit::analyze(larlite::storage_manager* storage) {

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


  // get a handle to the hits
  auto hitHandle = storage->get_data<larlite::event_hit>(_producer);

  // Clear out the hit data but reserve some space for the hits
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(hitHandle -> size());
    _maxCharge.at(p) = 0.0;
  }




  for (auto & hit : *hitHandle) {
    unsigned int view = hit.View();
    _dataByPlane.at(view).emplace_back(
      Hit(hit.WireID().Wire,
          hit.PeakTime(),
          hit.RMS(),
          hit.PeakAmplitude()));
    if (_dataByPlane.at(view).back()._charge > _maxCharge.at(view))
      _maxCharge.at(view) = _dataByPlane.at(view).back()._charge;
    // wireByPlane -> at(view).push_back(hit.WireID().Wire);
    // hitStartByPlane -> at(view).push_back(hit.PeakTime() - hit.RMS());
    // hitEndByPlane -> at(view).push_back(hit.PeakTime() + hit.RMS());

  }


  return true;
}

bool DrawHit::finalize() {

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




} // larlite

#endif
