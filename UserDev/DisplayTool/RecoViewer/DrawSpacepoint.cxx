#ifndef DRAWSPACEPOINT3D_CXX
#define DRAWSPACEPOINT3D_CXX

#include "DrawSpacepoint.h"
#include "LArUtil/GeometryHelper.h"

namespace evd {


DrawSpacepoint::DrawSpacepoint() {
  _name = "DrawSpacepoint";
  _fout = 0;

}

bool DrawSpacepoint::initialize() {
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;
}

bool DrawSpacepoint::analyze(larlite::storage_manager* storage) {

  // get a handle to the tracks
  auto spacepointHandle = storage->get_data<larlite::event_spacepoint>(_producer);

  geoHelper = larutil::GeometryHelper::GetME();

  // Clear out the data but reserve some space
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(spacepointHandle -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }

  Point2D point;

  // Populate the spacepoint vector:
  for (auto & spt : *spacepointHandle) {

    // A spacepoint is a 3D object.  So take it and project it into each plane:
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {


      try {
        point = geoHelper -> Point_3Dto2D(spt.XYZ(), p);
      }
      catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
      }
      _dataByPlane.at(p).push_back(point);


      // Determine if this hit should change the view range:
      if (point.w / geoHelper->WireToCm() > _wireRange.at(p).second)
        _wireRange.at(p).second = point.w / geoHelper->WireToCm();
      if (point.w / geoHelper->WireToCm() < _wireRange.at(p).first)
        _wireRange.at(p).first = point.w / geoHelper->WireToCm();
      if (point.t / geoHelper->TimeToCm() > _timeRange.at(p).second)
        _timeRange.at(p).second = point.t / geoHelper->TimeToCm();
      if (point.t / geoHelper->TimeToCm() < _timeRange.at(p).first)
        _timeRange.at(p).first = point.t / geoHelper->TimeToCm();
    }
  }


  return true;
}

bool DrawSpacepoint::finalize() {

  return true;
}

DrawSpacepoint::~DrawSpacepoint() {}


} // evd

#endif
