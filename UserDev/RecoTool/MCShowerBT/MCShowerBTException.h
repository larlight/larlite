/**
 * \file MCShowerBTException.h
 *
 * \ingroup MCShowerBT
 * 
 * \brief Class def header for exception classes in MCShowerBT package
 *
 * @author kazuhiro
 */

/** \addtogroup MCShowerBT

    @{*/
#ifndef RECOTOOL_MCSHOWERBTEXCEPTION_H
#define RECOTOOL_MCSHOWERBTEXCEPTION_H

#include <iostream>
#include <exception>

namespace larlite {
  /**
     \class MCShowerBTException
     Generic (base) exception class
  */
  class MCShowerBTException : public std::exception{

  public:

    MCShowerBTException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~MCShowerBTException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

