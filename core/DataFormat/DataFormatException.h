/**
 * \file DataFormatException.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for exception classes in DataFormat package
 *
 * @author kazuhiro
 */

/** \addtogroup DataFormat

    @{*/
#ifndef LARLITE_DATAFORMATEXCEPTION_H
#define LARLITE_DATAFORMATEXCEPTION_H

#include <iostream>
#include <exception>

namespace larlite {
  /**
     \class DataFormatException
     Generic (base) exception class
  */
  class DataFormatException : public std::exception{

  public:

    DataFormatException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~DataFormatException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

