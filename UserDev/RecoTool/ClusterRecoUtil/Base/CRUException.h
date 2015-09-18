/**
 * \file CRUException.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief Class def header for exception classes in ClusterRecoUtil package
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef RECOTOOL_CRUEXCEPTION_H
#define RECOTOOL_CRUEXCEPTION_H

#include <iostream>
#include <exception>

namespace cluster {
  /**
     \class CRUException
     Generic (base) exception class
  */
  class CRUException : public std::exception{

  public:

    CRUException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~CRUException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

