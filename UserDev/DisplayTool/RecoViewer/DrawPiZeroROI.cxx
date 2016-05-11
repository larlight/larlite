#ifndef DRAWPIZEROROI_CXX
#define DRAWPIZEROROI_CXX

#include "DrawPiZeroROI.h"
#include "DataFormat/PiZeroROI.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawPiZeroROI::DrawPiZeroROI()
  : RecoBase<roi2d>()
{
  _name = "DrawPiZeroROI";
  _fout = 0;
}

bool DrawPiZeroROI::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //


  // // Resize data holder to accommodate planes and wires:
  if (_dataByPlane.size() != geoService -> Nviews()) {
    // std::cout << "resizing vectors to: " << geoService -> Nviews() << std::endl;
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;

}

bool DrawPiZeroROI::analyze(larlite::storage_manager* storage) {

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

  // get a handle to the ROIs


  auto roiHandle = storage->get_data<larlite::event_PiZeroROI>(_producer);

  // Clear out the hit data but reserve some space for the rois
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(roiHandle -> size());
  }


  // Populate the roi vector:
  for (auto & roi : *roiHandle) {
    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      _dataByPlane.at(view).push_back(getroi2d(roi, view));
    }
  }


  return true;
}

bool DrawPiZeroROI::finalize() {

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



roi2d DrawPiZeroROI::getroi2d(larlite::PiZeroROI roi, unsigned int plane) {


  //  auto detProp = larutil::DetectorProperties::GetME();

  roi2d result;
  result._is_good = false;
  result._plane = plane;
  // Fill out the parameters of the roi

  //Fill out the things
  result._maxmaxPoint = larutil::Point2D();
  result._maxmaxPoint.w = roi.GetWireROI().at(plane).second;
  result._maxmaxPoint.t = roi.GetTimeROI().at(plane).second;

  result._pi0maxmaxPoint = larutil::Point2D();
  result._pi0maxmaxPoint.w = roi.GetPiZeroWireROI().at(plane).second;
  result._pi0maxmaxPoint.t = roi.GetPiZeroTimeROI().at(plane).second;

  result._minminPoint = larutil::Point2D();
  result._minminPoint.w = roi.GetWireROI().at(plane).first;
  result._minminPoint.t = roi.GetTimeROI().at(plane).first;

  result._pi0minminPoint = larutil::Point2D();
  result._pi0minminPoint.w = roi.GetPiZeroWireROI().at(plane).first;
  result._pi0minminPoint.t = roi.GetPiZeroTimeROI().at(plane).first;

  result._vtxPoint = larutil::Point2D();
  result._vtxPoint.t = roi.GetVertex().at(plane).first;
  result._vtxPoint.w = roi.GetVertex().at(plane).second;

  result._trkendPoint = larutil::Point2D();
  result._trkendPoint.t = roi.GetTrackEnd().at(plane).first;
  result._trkendPoint.w = roi.GetTrackEnd().at(plane).second;

  // save neutrino vertex
  auto nuvtx  = roi.GetNeutrinoVertex();
  auto point  = geoHelper->Point_3Dto2D( nuvtx , plane);
  result._nuvtxPoint = point;

  result._is_good = true;
  return result;
}



} // larlite

#endif
