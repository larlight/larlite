/**
 * \file LArUtilException.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for exception classes in LArUtil package
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include <iostream>
#include <exception>

namespace larutil {
  /**
     \class LArUtilException
     Generic (base) exception class
  */
  class LArUtilException : public std::exception{

  public:

    LArUtilException(std::string msg="") : std::exception()
    {
      _msg = "\033[93m";
      _msg += msg;
      _msg += "\033[00m";
    }

    virtual ~LArUtilException() throw(){};
    virtual const char* what() const throw() 
    { return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

