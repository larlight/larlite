/**
 * \file RecoBase.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class RecoBase
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef RECOBASE_H
#define RECOBASE_H

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
   \class RecoBase
   User defined class RecoBase ... these comments are used to generate
   doxygen documentation!
 */
namespace evd {

template <class DATA_TYPE >
class RecoBase {

public:

  /// Default constructor
  RecoBase();

  /// Default destructor
  ~RecoBase() {}

  std::pair<float, float> getWireRange(size_t p);
  std::pair<float, float> getTimeRange(size_t p);

  void setProducer(std::string s);

  const std::vector<DATA_TYPE> & getDataByPlane(size_t p);
  // PyObject * getDataByPlane(size_t p);

protected:

  void _init_base();

  const larutil::Geometry * geoService;
  const larutil::GeometryHelper * geoHelper;
  const larutil::DetectorProperties * detProp;

  std::string _producer;

  // Store the reco data to draw;
  std::vector <std::vector < DATA_TYPE > > _dataByPlane;

  // Store the bounding parameters of interest:
  // highest and lowest wire, highest and lowest time
  // Have to sort by plane still

  std::vector<std::pair<float, float> > _wireRange;
  std::vector<std::pair<float, float> > _timeRange;
};


template <class DATA_TYPE>
RecoBase <DATA_TYPE>::RecoBase() {
  geoService = larutil::Geometry::GetME();
  geoHelper = larutil::GeometryHelper::GetME();
  detProp = larutil::DetectorProperties::GetME();
  // Set up default values of the _wire and _time range
  _wireRange.resize(geoService -> Nviews());
  _timeRange.resize(geoService -> Nviews());
  for (size_t view = 0; view < geoService -> Nviews(); view ++) {
    _wireRange.at(view).first  = 0;
    _wireRange.at(view).second = geoService -> Nwires(view);
    _timeRange.at(view).first  = 0;
    _timeRange.at(view).second = detProp -> NumberTimeSamples();
  }
  // import_array();
}

template <class DATA_TYPE>
void RecoBase <DATA_TYPE>::setProducer(std::string s) {
  _producer = s;
}

template <class DATA_TYPE>
std::pair<float, float> RecoBase<DATA_TYPE>::getWireRange(size_t p) {
  static std::pair<float, float> returnNull;
  if (p >= geoService->Nviews() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      return _wireRange.at(p);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return returnNull;
    }
  }
}


template <class DATA_TYPE>
std::pair<float, float> RecoBase<DATA_TYPE>::getTimeRange(size_t p) {
  static std::pair<float, float> returnNull;
  if (p >= geoService->Nviews() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      return _timeRange.at(p);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return returnNull;
    }
  }
}

template <class DATA_TYPE>
const std::vector<DATA_TYPE> & RecoBase<DATA_TYPE>::getDataByPlane(size_t p) {
  static std::vector<DATA_TYPE> returnNull;
  if (p >= geoService->Nviews() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      return _dataByPlane.at(p);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return returnNull;
    }
  }
}

// template <class DATA_TYPE>
// PyObject * RecoBase<DATA_TYPE>::getDataByPlane(size_t p) {
//   PyObject* pvec = PyList_New(_dataByPlane.at(p).size());
//   if (p >= geoService->Nviews() ) {
//     std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
//     Py_DECREF(pvec);
//     throw std::exception();
//   }
//   else {
//     // try {
//       for (size_t i = 0; i < _dataByPlane.at(p).size(); ++i) {

//         if (PyList_SetItem(pvec, i, &(_dataByPlane.at(p)[i]))) {

//           Py_DECREF(pvec);
//           throw std::exception();
//         }
//       }
//       return pvec;
//     // }
//     // catch (const std::exception& e) {
//     //   std::cerr << e.what() << '\n';
//     // }
//   }
// }


} // evd

#endif
/** @} */ // end of doxygen group

