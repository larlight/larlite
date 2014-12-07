/**
 * \file SPAException.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for exception classes in LArUtil package
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/
#ifndef SPAEXCEPTION_H
#define SPAEXCEPTION_H

#include <iostream>
#include <exception>

namespace sptool {
  /**
     \class SPAException
     Generic (base) exception class
  */
  class SPAException : public std::exception{

  public:

    SPAException(std::string msg="") : std::exception()
    {
      _msg = "\033[93m EXCEPTION \033[00m\033[95m";
      _msg += msg;
      _msg += "\033[00m\n";
    }

    virtual ~SPAException() throw(){};
    virtual const char* what() const throw() 
    { return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

