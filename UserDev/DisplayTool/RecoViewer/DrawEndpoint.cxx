#ifndef EVD_DRAWENDPOINT_CXX
#define EVD_DRAWENDPOINT_CXX

#include "DrawEndpoint.h"
#include "DataFormat/endpoint2d.h"

namespace evd {

DrawEndpoint::DrawEndpoint() {
  _name = "DrawEndpoint";
  _fout = 0;
}

bool DrawEndpoint::initialize() {

  //
  // This function is called in the beginning of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //

  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;
}

bool DrawEndpoint::analyze(larlite::storage_manager* storage) {

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

  // clear the spots that hold the data:
  // Obtain event-wise data object pointers
  //


  auto ev_end2d = storage->get_data<larlite::event_endpoint2d>(_producer);



  // Clear out the data but reserve some space
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(ev_end2d -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }

  if (!ev_end2d)
    return false;
  if (!ev_end2d->size()) {
    print(larlite::msg::kWARNING, __FUNCTION__,
          Form("Skipping event %d since no endpoint2d found...", ev_end2d->event_id()));
    return false;
  }



  // Loop over the endpoint2ds and fill the necessary vectors.

  // larutil::PxPoint point;
  // double * xyz = new double[3];

  for (auto & endpoint2d : * ev_end2d) {
    // for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
    int p = endpoint2d.View();

    // Add it to the data:
    _dataByPlane.at(p).emplace_back(
      Endpoint2D(endpoint2d.Wire(),
                 endpoint2d.DriftTime(),
                 endpoint2d.Charge(),
                 endpoint2d.Strength() ));

    // Determine if this endpoint2d should change the view range:
    if (endpoint2d.Wire() > _wireRange.at(p).second)
      _wireRange.at(p).second = endpoint2d.Wire();
    if (endpoint2d.Wire() < _wireRange.at(p).first)
      _wireRange.at(p).first = endpoint2d.Wire();
    if (endpoint2d.DriftTime() > _timeRange.at(p).second)
      _timeRange.at(p).second = endpoint2d.DriftTime();
    if (endpoint2d.DriftTime() < _timeRange.at(p).first)
      _timeRange.at(p).first = endpoint2d.DriftTime();
    // }
  }


  return true;
}

bool DrawEndpoint::finalize() {

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
  //


  return true;
}


}
#endif
