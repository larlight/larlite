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
#ifndef RECOTOOL_CRU3DEXCEPTION_H
#define RECOTOOL_CRU3DEXCEPTION_H

#include <iostream>
#include <exception>

namespace cluster3D {
  /**
     \class CRUException
     Generic (base) exception class
  */
  class CRU3DException : public std::exception{

  public:

    CRU3DException(std::string msg="") : std::exception(), _msg(msg)
    {}

    virtual ~CRU3DException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

