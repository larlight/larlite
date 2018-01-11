/**
 * \file ViewerException.h
 *
 * \ingroup CMTool
 * 
 * \brief Class def header for exception classes in CMToolApp package
 *
 * @author kazuhiro
 */

/** \addtogroup CMTool

    @{*/
#ifndef RECOTOOL_VIEWEREXCEPTION_H
#define RECOTOOL_VIEWEREXCEPTION_H

#include <iostream>
#include <exception>

namespace cluster {
  /**
     \class ViewerException
     Generic (base) exception class
  */
  class ViewerException : public std::exception{

  public:

    ViewerException(std::string msg="") : std::exception(),
					  _msg("")
    {
      _msg += "\033[93m";
      _msg += "[EXCEPTION] : ";
      _msg += "\033[0m";
      _msg += "\033[91m";
      _msg += msg;
      _msg += "\033[0m";
    }

    virtual ~ViewerException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

