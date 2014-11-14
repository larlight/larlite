/**
 * \file ShowerAnaException.h
 *
 * \ingroup ShowerStudy 
 * 
 * \brief Class def header for exception classes in ShowerStudy package
 *
 * @author kazuhiro
 */

/** \addtogroup ClusterRecoUtil

    @{*/
#ifndef SHOWERANAEXCEPTION_H
#define SHOWERANAEXCEPTION_H

#include <iostream>
#include <exception>

namespace showerana {
  /**
     \class ShowerAnaException
     Generic (base) exception class
  */
  class ShowerAnaException : public std::exception{

  public:
    
    ShowerAnaException(std::string msg="") : std::exception(), _msg(msg)
    {}
    
    virtual ~ShowerAnaException() throw(){};
    virtual const char* what() const throw() 
    {return _msg.c_str(); }

  private:
    
    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

