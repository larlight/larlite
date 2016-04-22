
#ifndef LARLITE_DRAWWIRE_CXX
#define LARLITE_DRAWWIRE_CXX


#include "LArUtil/DetectorProperties.h"
#include "DrawWire.h"
#include "DataFormat/wire.h"

namespace evd {

DrawWire::DrawWire() {
  _name = "DrawWire";
  producer = "caldata";


}

bool DrawWire::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //
  //


  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    setXDimension(geoService->Nwires(p), p);
    setYDimension(detProp -> ReadOutWindowSize(), p);
  }
  initDataHolder();



  return true;

}

bool DrawWire::analyze(larlite::storage_manager* storage) {

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

  // This is an event viewer.  In particular, this handles raw wire signal drawing.
  // So, obviously, first thing to do is to get the wires.
  auto WireDigitHandle = storage->get_data<larlite::event_wire>(producer);

  _planeData.clear();
  initDataHolder();


  for (auto const& wire : *WireDigitHandle) {
    unsigned int ch = wire.Channel();
    unsigned int detWire = geoService->ChannelToWire(ch);
    unsigned int plane = geoService->ChannelToPlane(ch);
    int offset = detWire * detProp -> ReadOutWindowSize();


    for (auto & iROI : wire.SignalROI().get_ranges()) {
      // for (auto iROI = wire.SignalROI().begin_range(); wire.SignalROI().end_range(); ++iROI) {
      const int FirstTick = iROI.begin_index();
      size_t i = 0;
      for (float ADC : iROI) {
        _planeData.at(plane).at(offset + FirstTick + i) = ADC;
        i ++;
      }


    }
  }

  return true;
}

bool DrawWire::finalize() {

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

}

#endif