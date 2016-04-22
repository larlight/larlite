/**
 * \file RecoBase3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class RecoBase3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef RECOBASE3D_H
#define RECOBASE3D_H

#include <iostream>

struct _object;
typedef _object PyObject;

// #ifndef __CINT__
// #include "Python.h"
// #include "numpy/arrayobject.h"
// #endif

#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"


/**
   \class RecoBase3D
   User defined class RecoBase3D ... these comments are used to generate
   doxygen documentation!
 */
namespace evd {

template <class DATA_TYPE >
class RecoBase3D {

public:

  /// Default constructor
  RecoBase3D();

  /// Default destructor
  virtual ~RecoBase3D() {}

  std::pair<float, float> getXRange();
  std::pair<float, float> getYRange();
  std::pair<float, float> getZRange();

  void setProducer(std::string s);

  const std::vector<DATA_TYPE> & getData();
  // PyObject * getDataByPlane(size_t p);

protected:

  void _init_base();

  const larutil::Geometry * geoService;
  const larutil::GeometryHelper * geoHelper;
  const larutil::DetectorProperties * detProp;

  std::string _producer;

  // Store the reco data to draw;
  std::vector < DATA_TYPE > _data;

  // Store the bounding parameters of interest:
  // highest and lowest wire, highest and lowest time
  // Have to sort by plane still

  std::pair<float, float>  _xRange;
  std::pair<float, float>  _yRange;
  std::pair<float, float>  _zRange;
};


template <class DATA_TYPE>
RecoBase3D <DATA_TYPE>::RecoBase3D() {
  geoService = larutil::Geometry::GetME();
  geoHelper = larutil::GeometryHelper::GetME();
  detProp = larutil::DetectorProperties::GetME();
  // Set up default values of the _wire and _time range
  _xRange.first  = 0;
  _xRange.second = 2 * geoService -> DetHalfWidth();
  _yRange.first  = -geoService -> DetHalfHeight();
  _yRange.second =  geoService -> DetHalfHeight();
  _zRange.first =  0;
  _zRange.second =  geoService -> DetLength();
  // import_array();
}

template <class DATA_TYPE>
void RecoBase3D <DATA_TYPE>::setProducer(std::string s) {
  _producer = s;
}

template <class DATA_TYPE>
std::pair<float, float> RecoBase3D<DATA_TYPE>::getXRange() {
  static std::pair<float, float> returnNull;
  try {
    return _xRange;
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return returnNull;
  }
}


template <class DATA_TYPE>
std::pair<float, float> RecoBase3D<DATA_TYPE>::getYRange() {
  static std::pair<float, float> returnNull;
  try {
    return _yRange;
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return returnNull;
  }
}

template <class DATA_TYPE>
std::pair<float, float> RecoBase3D<DATA_TYPE>::getZRange() {
  static std::pair<float, float> returnNull;
  try {
    return _zRange;
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return returnNull;
  }
}


template <class DATA_TYPE>
const std::vector<DATA_TYPE> & RecoBase3D<DATA_TYPE>::getData() {
  static std::vector<DATA_TYPE> returnNull;
  try {
    return _data;
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return returnNull;
  }
}


} // evd

#endif
/** @} */ // end of doxygen group

