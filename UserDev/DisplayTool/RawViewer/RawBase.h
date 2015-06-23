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


    // The following two functions are the interface
    // to the outside world in terms of passing data out.
    // Do not change their form without good reason!
    // If you are concerned about the speed of passing this 
    // vector, I think it is OK.  It is passed by reference 
    // and so later, a c++ -> numpy converter could turn this
    // data into numpy array.  This seems ideal; this class
    // here could be the owner of the data, responsible for
    // memory management, while numpy provides the streamlined
    // interface for python for drawing.


    // Function to get the data by plane:
    const std::vector<std::vector<float>> & getDataByPlane(unsigned int p) const;

    // Get just a single wire's worth of data
    const std::vector<float> & getWireData(unsigned int plane, unsigned int wire) const;

  protected:

    std::vector<std::vector<std::vector<float>>> * wiredata;

    const larutil::Geometry * geoService;

    std::string producer;

  };
} // evd

#endif
/** @} */ // end of doxygen group 

