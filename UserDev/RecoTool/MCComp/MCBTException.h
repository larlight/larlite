/**
 * \file MCBTException.h
 *
 * \ingroup MCComp
 * 
 * \brief Class def header for exception classes in MCComp package
 *
 * @author kazuhiro
 */

/** \addtogroup MCComp

    @{*/
#ifndef RECOTOOL_MCBTEXCEPTION_H
#define RECOTOOL_MCBTEXCEPTION_H

#include <iostream>
#include <exception>

namespace btutil {

  /**
     \class MCBTException
     Generic (base) exception class
  */
  class MCBTException : public std::exception{

  public:

    MCBTException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~MCBTException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

