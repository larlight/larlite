/**
 * \file RawBase.h
 *
 * \ingroup RawViewer
 * 
 * \brief Class def header for a class RawBase
 *
 * @author cadams
 */

/** \addtogroup RawViewer

    @{*/
#ifndef RAWBASE_H
#define RAWBASE_H

#include <iostream>
#include <vector>
#include "LArUtil/Geometry.h"
#include "LArUtil/DetectorProperties.h"

struct _object;
typedef _object PyObject;



#ifndef __CINT__
#include "Python.h"
#include "numpy/arrayobject.h"
#endif



namespace evd {

  /**
     \class RawBase
     This is the base class for drawing raw wire information.
     It contains the data and the methods to get the data
   */
  class RawBase{

  public:

    /// Default constructor
    RawBase();

    /// Default destructor
    ~RawBase();

    // This function sets the input target
    // for larlite, this can be used to set the producer
    // for lariat, this can be used to set the file
    void setInput(std::string s){producer = s;}

    // This class has two outputs.
    // First, a user can get the plane data as a vector of float
    // Which means the user may have to know how to convert that back into
    // a 2D array
    // Alternatively, and because this is meant to be used as python,
    // you can get the numpy array of the plane of data, which is 2D

    // You are responsible for setting the dimensions of the planes too
    // There is no requirement they are uniform, but you should make sure to initialize
    // them properly with the following functions:
    void setXDimension(unsigned int x_dim, unsigned int plane);
    void setYDimension(unsigned int y_dim, unsigned int plane);
    void setPedestal(float pedestal, unsigned int plane);



    // Function to get the array of data
    PyObject * getArrayByPlane(unsigned int p);

    // Function to get the data by plane:
    const std::vector<float> & getDataByPlane(unsigned int p) const;

  protected:

    // sets up the _plane data object
    void initDataHolder();


    // This section holds the images for the data (wire, raw digit, etc)
    std::vector<std::vector<float > > _planeData;

    // these two objects hold the dimensions of the data:
    std::vector<unsigned int> _x_dimensions;
    std::vector<unsigned int> _y_dimensions;

    std::vector<float> _pedestals;

    const larutil::Geometry * geoService;
    const larutil::DetectorProperties * detProp;

    std::string producer;

  };
} // evd

#endif
/** @} */ // end of doxygen group 

