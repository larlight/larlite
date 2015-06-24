/**
 * \file Converter.h
 *
 * \ingroup RawViewer
 * 
 * \brief Class def header for a class Converter
 *
 * @author cadams
 */

/** \addtogroup RawViewer

    @{*/
#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>

struct _object;
typedef _object PyObject;


#ifndef __CINT__
// You have to hide native Python header include from CINT
#include "Python.h"
#endif


#include <vector>
#include <string>

/**
   \class Converter
   User defined class Converter ... these comments are used to generate
   doxygen documentation!
 */
namespace evd {

class Converter{

public:

  /// Default constructor
  Converter(){}

  /// Default destructor
  virtual ~Converter(){}

  PyObject* Convert(const std::vector<std::string>& str_array) const;
  PyObject* Convert(const std::vector<float>& flt_array) const;
  PyObject* Convert(const std::vector<std::vector<float>>& fltflt_array) const;
  PyObject* Convert(const std::vector<int>& flt_array) const;
  PyObject* Convert(const std::vector<std::vector<int>>& fltflt_array) const;
  PyObject* ConvertShort(const std::vector<unsigned short>& flt_array) const;
  PyObject* ConvertShort(const std::vector<std::vector<unsigned short>>& fltflt_array) const;

};

} // larlite


#endif
/** @} */ // end of doxygen group 

