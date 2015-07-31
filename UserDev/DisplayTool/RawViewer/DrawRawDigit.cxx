#ifndef LARLITE_DRAWRAWDIGIT_CXX
#define LARLITE_DRAWRAWDIGIT_CXX

#include "DrawRawDigit.h"
#include "DataFormat/rawdigit.h"

namespace evd {

  DrawRawDigit::DrawRawDigit(){ 
    _name="DrawRawDigit"; 
    _fout=0;
    // wiredata = new std::vector<std::vector<std::vector<float> > > ;
    producer = "daq";
  }

  bool DrawRawDigit::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    // Initialize the geoService object:

    // Initialize data holder:
    // Resize data holder to accomodate planes and wires:
    if (wiredata -> size() != geoService -> Nviews())
      wiredata->resize(geoService -> Nviews());
     
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      if (wiredata->at(p).size() != geoService->Nwires(p) )
        wiredata->at(p).resize(geoService->Nwires(p));
    }

    // std::cout << "\n\nCompleted initialize.\n\n";

    return true;

  }
  
  bool DrawRawDigit::analyze(larlite::storage_manager* storage) {
  
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
    auto RawDigitHandle = storage->get_data<larlite::event_rawdigit>(producer);
    

    for (auto const& rawdigit: *RawDigitHandle){
        unsigned int ch = rawdigit.Channel();
	if (ch >= 8254) continue;
	// convert short ADCs to float
	std::vector<float> ADCs;
	auto const& ADCtmp = rawdigit.ADCs();
	for (auto const& ADC : ADCtmp)
	  ADCs.push_back((float)ADC);
	wiredata->at(geoService->ChannelToPlane(ch))[geoService->ChannelToWire(ch)] = ADCs;
    }


    return true;
  }

  bool DrawRawDigit::finalize() {

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
  
    delete wiredata;

    return true;
  }


}
#endif
