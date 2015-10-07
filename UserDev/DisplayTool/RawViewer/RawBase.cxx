#ifndef RAWBASE_CXX
#define RAWBASE_CXX

#include "RawBase.h"

namespace evd {

RawBase::RawBase() {
  geoService = larutil::Geometry::GetME();
  detProp = larutil::DetectorProperties::GetME();
  import_array();
}

RawBase::~RawBase() {

}


const std::vector<float> & RawBase::getDataByPlane(unsigned int p) const {
  static std::vector<float> returnNull;
  if (p >= geoService->Nviews()) {
    std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      return _planeData.at(p);
    }
    catch ( ... ) {
      std::cerr << "WARNING:  REQUEST FOR PLANE FOR WHICH THERE IS NOT WIRE DATA.\n";
      return returnNull;
    }
  }

}

PyObject * RawBase::getArrayByPlane(unsigned int p) {

  PyObject * returnNull = nullptr;
  if (p >= geoService->Nviews()) {
    std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      // Convert the wire data to numpy arrays:
      int n_dim = 2;
      int * dims = new int[n_dim];
      dims[0] = _x_dimensions.at(p);
      dims[1] = _y_dimensions.at(p);
      int data_type = PyArray_FLOAT;

      return (PyObject *) PyArray_FromDimsAndData(n_dim, dims, data_type, (char*) & ((_planeData.at(p))[0]) );
    }
    catch ( ... ) {
      std::cerr << "WARNING:  REQUEST FOR PLANE FOR WHICH THERE IS NOT WIRE DATA.\n";
      return returnNull;
    }
  }

}


void RawBase::setXDimension(unsigned int x_dim, unsigned int plane) {
  if (_x_dimensions.size() < plane + 1) {
    _x_dimensions.resize(plane + 1);
  }
  _x_dimensions.at(plane) = x_dim;
  return;
}
void RawBase::setYDimension(unsigned int y_dim, unsigned int plane) {
  if (_y_dimensions.size() < plane + 1) {
    _y_dimensions.resize(plane + 1);
  }
  _y_dimensions.at(plane) = y_dim;
  return;
}

void RawBase::setPedestal(float pedestal, unsigned int plane) {
  if (_pedestals.size() < plane + 1) {
    _pedestals.resize(plane + 1);
  }
  _pedestals.at(plane) = pedestal;
  return;
}


void RawBase::initDataHolder() {
  _planeData.resize(_x_dimensions.size());
  for (int i = 0; i < _x_dimensions.size(); i ++ ) {
    _planeData.at(i).resize(_x_dimensions.at(i) * _y_dimensions.at(i));
  }
  return;
}




} // evd


#endif
