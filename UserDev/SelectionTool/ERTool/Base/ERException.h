/**
 * \file ERException.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for exception classes in ERTool package
 *
 * @author kazuhiro
 */

/** \addtogroup ERTool

    @{*/
#ifndef ERTOOL_EXCEPTION_H
#define ERTOOL_EXCEPTION_H

#include <iostream>
#include <exception>

namespace ertool {
  /**
     \class ERException
     Generic (base) exception class
  */
  class ERException : public std::exception{

  public:

    ERException(std::string msg="") : std::exception()
    {
      _msg = "\033[93m EXCEPTION \033[00m\033[95m";
      _msg += msg;
      _msg += "\033[00m\n";
    }

    virtual ~ERException() throw(){};
    virtual const char* what() const throw() 
    { return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

