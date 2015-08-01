#ifndef CONVERTER_CXX
#define CONVERTER_CXX

#include "Converter.h"
#include <iostream>
#include <exception>

namespace evd {

  // PyObject* Converter::Convert(const std::vector<::evd::Shower2d> & _array) const{
  //   PyObject* pvec = PyList_New(_array.size());

  //   for(size_t i=0; i<_array.size(); ++i) {

  //     if(PyList_SetItem(pvec, i, _array[i])) {

  //       Py_DECREF(pvec);
  //     }
  //   }
  //   return pvec;
  // }

  PyObject* Converter::Convert(const std::vector<std::string>& str_array) const
  {
    PyObject* pvec = PyList_New(str_array.size());

    for(size_t i=0; i<str_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, PyString_FromString(str_array[i].c_str()))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }

  PyObject* Converter::Convert(const std::vector<float>& flt_array) const
  {
    PyObject* pvec = PyList_New(flt_array.size());

    for(size_t i=0; i<flt_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, PyFloat_FromDouble(flt_array[i]))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }

  PyObject* Converter::Convert(const std::vector<std::vector<float>>& fltflt_array) const
  {
    PyObject* pvec = PyList_New(fltflt_array.size());

    for(size_t i=0; i<fltflt_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, Convert(fltflt_array[i]))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }

  PyObject* Converter::Convert(const std::vector<int>& int_array) const
  {
    PyObject* pvec = PyList_New(int_array.size());

    for(size_t i=0; i<int_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, PyFloat_FromDouble(int_array[i]))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }

  PyObject* Converter::Convert(const std::vector<std::vector<int>>& intint_array) const
  {
    PyObject* pvec = PyList_New(intint_array.size());

    for(size_t i=0; i<intint_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, Convert(intint_array[i]))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }

  PyObject* Converter::ConvertShort(const std::vector<unsigned short>& us_array) const{
    PyObject* pvec = PyList_New(us_array.size());

    for(size_t i=0; i<us_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, PyLong_FromLong(us_array[i]))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }

  PyObject* Converter::ConvertShort(const std::vector<std::vector<unsigned short>>& usus_array) const{
    PyObject* pvec = PyList_New(usus_array.size());

    for(size_t i=0; i<usus_array.size(); ++i) {

      if(PyList_SetItem(pvec, i, ConvertShort(usus_array[i]))) {

        Py_DECREF(pvec);
        std::cerr<<"<<Convert>> failed!"<<std::endl;
        throw std::exception();
      }
    }
    return pvec;
  }




}


#endif
