/**
 * \file InvalidWireError.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for exception class for InvalidWireError
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef INVALIDWIREEXCEPTION_H
#define INVALIDWIREEXCEPTION_H

#include <iostream>
#include <exception>

namespace larutil {
  /**
     \class InvalidWireError
  */
  class InvalidWireError : public std::exception{

  public:

    InvalidWireError(std::string msg="") : std::exception()
    {
      better_wire_number = 0;
      _msg = "\033[93m";
      _msg += msg;
      _msg += "\033[00m";
    }

    virtual ~InvalidWireError() throw(){};
    virtual const char* what() const throw() 
    { return _msg.c_str(); }

    unsigned int better_wire_number;

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

