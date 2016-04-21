/**
 * \file ShowerRecoException.h
 *
 * \ingroup ShowerReco3D
 *
 * \brief Class def header for exception classes in ShowerReco3D package
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_SHOWERRECOEXCEPTION_H
#define RECOTOOL_SHOWERRECOEXCEPTION_H

#include <iostream>
#include <exception>

namespace showerreco {
/**
   \class ShowerRecoException
   Generic (base) exception class
*/
class ShowerRecoException : public std::exception {

public:

  ShowerRecoException(std::string msg = "") : std::exception()
  {
    _msg = "\033[93m";
    _msg += msg;
    _msg += "\033[00m";
  }

  virtual ~ShowerRecoException() throw() {};
  virtual const char* what() const throw()
  { return _msg.c_str(); }

private:

  std::string _msg;
};

}
#endif
/** @} */ // end of doxygen group

