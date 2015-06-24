/**
 * \file CToNumpy.h
 *
 * \ingroup Utils
 * 
 * \brief Class def header for a class CToNumpy
 *
 * @author cadams
 */

/** \addtogroup Utils

    @{*/
#ifndef CTONUMPY_H
#define CTONUMPY_H

struct _object;
typedef _object PyObject;
// typedef _object PyArrayObject;

#include <iostream>
#include <vector>

#ifndef __CINT__
#include "Python.h"
#include "numpy/arrayobject.h"
#endif



/**
   \class CToNumpy
   User defined class CToNumpy ... these comments are used to generate
   doxygen documentation!
 */
class CToNumpy{

public:

  /// Default constructor
  CToNumpy();

  /// Default destructor
  ~CToNumpy(){}

  // template <class T>
  PyObject* Convert(  std::vector<float> * _array) const;
  PyObject* Convert( std::vector<std::vector<float>> * _array) const;

  std::vector<float> getTestVector();
  std::vector<std::vector<float>> getTestVector2d();

private:
  std::vector<float> vec;
  std::vector<std::vector<float>> vec2d;
};

#endif
/** @} */ // end of doxygen group 

