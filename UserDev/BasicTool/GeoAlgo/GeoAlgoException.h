/**
 * \file GeoAlgoException.h
 *
 * \ingroup GeoAlgo
 * 
 * \brief Class def header for a class GeoAlgoException
 *
 * @author kazu 
 */

/** \addtogroup GeoAlgo

    @{*/
#ifndef LARLITE_GEOALGOEXCEPTION_H
#define LARLITE_GEOALGOEXCEPTION_H

#include <iostream>
#include <exception>

namespace geoalgo {
/**
   \class GeoAlgoException
   User defined class GeoAlgoException ... these comments are used to generate
   doxygen documentation!
 */
    
 class GeoAlgoException : public std::exception{

  public:
        
    GeoAlgoException(std::string msg="") : std::exception(), _msg(msg)
    {}  
    
    virtual ~GeoAlgoException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:
    
    std::string _msg;
 };

}

#endif
/** @} */ // end of doxygen group 

