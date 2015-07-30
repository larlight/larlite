#ifndef CTONUMPY_CXX
#define CTONUMPY_CXX

#include "CToNumpy.h"

  CToNumpy::CToNumpy(){
    Py_InitModule("CToNumpy",NULL);
    import_array(); 
  }

  // template <class T>
  // PyArrayObject* CToNumpy::Convert(const std::vector<T>& _array) const
  // {
  //   PyObject* pvec = PyList_New(_array.size());

  //   // for(size_t i=0; i<_array.size(); ++i) {

  //     // if(PyList_SetItem(pvec, i, PyString_FromString(_array[i].c_str()))) {

  //       // Py_DECREF(pvec);
  //       // std::cerr<<"<<Convert>> failed!"<<std::endl;
  //       // throw std::exception();
  //     // }
  //   // }
  //   return (PyArrayObject*) pvec;
  // }


  std::vector<float> CToNumpy::getTestVector(){
    if (vec.size() == 0){
      vec.resize(100);
      for (int i = 0; i < vec.size(); i ++) vec[i] = i;
    }
    return vec;
  }

  std::vector<std::vector<float> > CToNumpy::getTestVector2d(){
    if (vec2d.size() == 0){
      vec2d.resize(7);
      vec2d[0] = getTestVector();
      vec2d[1] = getTestVector();
      vec2d[2] = getTestVector();
      vec2d[3] = getTestVector();
      vec2d[4] = getTestVector();
      vec2d[5] = getTestVector();
      vec2d[6] = getTestVector();
    }
    return vec2d;
  }

  PyObject* CToNumpy::Convert(  std::vector<float> * _array) const{

    int n_dim = 1;
    int * dims = new int[n_dim];
    dims[0] = _array->size();
    int data_type = PyArray_FLOAT;
    // PyObject * retVec;
    PyArrayObject * retVec = (PyArrayObject*) PyArray_FromDimsAndData(n_dim, dims, data_type, (char*) &((*_array)[0]));
     // =  PyArray_FromDims(n_dim, dims, data_type);
    // return PyArray_Return(retVec);
    return (PyObject*) retVec;

  }

  PyObject* CToNumpy::Convert(  std::vector<std::vector<float> > * _array) const{

    int n_dim = 2;
    int * dims = new int[n_dim];
    dims[0] = _array->size();
    dims[1] = _array->front().size();
    int data_type = PyArray_FLOAT;
    // PyObject * retVec;
    PyArrayObject * retVec = (PyArrayObject*) PyArray_FromDims(n_dim, dims, data_type);
    // Loop over the first dim to get the inner data:

    // Modify the outermost stride to accomodate the vector header info:
     // =  PyArray_FromDims(n_dim, dims, data_type);
    // return PyArray_Return(retVec);
    return (PyObject*) retVec;

  }

  template <class T>
  PyObject* CToNumpy::Convert( std::vector<T> * _array) const{
    PyObject* pvec = PyList_New(_array -> size());

    for(size_t i=0; i<_array -> size(); ++i) {

      if(PyList_SetItem(pvec, i, _array[i])) {

        Py_DECREF(pvec);
      }
    }
    return pvec;
  }

#endif
