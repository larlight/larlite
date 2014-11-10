/**
 * \file CMToolAnaException.h
 *
 * \ingroup CMToolAna
 * 
 * \brief Class def header for exception classes in CMToolAna package
 *
 * @author kazuhiro
 */

/** \addtogroup CMToolAna

    @{*/
#ifndef RECOTOOL_CMTOOLANAEXCEPTION_H
#define RECOTOOL_CMTOOLANAEXCEPTION_H

#include <iostream>
#include <exception>

namespace larlite {
  /**
     \class CMToolAnaException
     Generic (base) exception class
  */
  class CMToolAnaException : public std::exception{

  public:

    CMToolAnaException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~CMToolAnaException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

