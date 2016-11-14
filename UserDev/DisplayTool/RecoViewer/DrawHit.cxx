#ifndef EVD_DRAWHIT_CXX
#define EVD_DRAWHIT_CXX

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
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }




  for (auto & hit : *hitHandle) {
    unsigned int view = hit.View();
    _dataByPlane.at(view).emplace_back(
      Hit2D(hit.WireID().Wire,
            hit.PeakTime(),
            hit.Integral(),
            hit.RMS(),
            hit.StartTick(),
            hit.PeakTime(),
            hit.EndTick(),
            hit.PeakAmplitude()
           ));
    if (_dataByPlane.at(view).back()._charge > _maxCharge.at(view))
      _maxCharge.at(view) = _dataByPlane.at(view).back()._charge;
    // Check the auto range values:
    if (_dataByPlane.at(view).back().wire() < _wireRange.at(view).first) {
      _wireRange.at(view).first = _dataByPlane.at(view).back().wire();
    }
    if (_dataByPlane.at(view).back().wire() > _wireRange.at(view).second) {
      _wireRange.at(view).second = _dataByPlane.at(view).back().wire();
    }
    if (_dataByPlane.at(view).back().time() < _timeRange.at(view).first) {
      _timeRange.at(view).first = _dataByPlane.at(view).back().time();
    }
    if (_dataByPlane.at(view).back().time() > _timeRange.at(view).second) {
      _timeRange.at(view).second = _dataByPlane.at(view).back().time();
    }
    // wireByPlane -> at(view).push_back(hit.WireID().Wire);
    // hitStartByPlane -> at(view).push_back(hit.PeakTime() - hit.RMS());
    // hitEndByPlane -> at(view).push_back(hit.PeakTime() + hit.RMS());

  }


  return true;
}

float DrawHit::maxCharge(size_t p) {
  if (p >= geoService->Nviews() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    return 1.0;
  }
  else {
    try {
      return _maxCharge.at(p);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1.0;
    }
  }


}

std::vector<Hit2D> DrawHit::getHitsOnWirePlane(size_t wire, size_t plane) {
  std::vector<Hit2D> result;

  if (plane >= geoService->Nviews() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << plane << std::endl;
    return result;
  }
  else {
    for (auto & hit : _dataByPlane.at(plane)) {
      if (hit.wire() == wire)
        result.emplace_back(hit);
    }
  }

  return result;
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
