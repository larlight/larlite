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

    ERException(const std::string& msg="");

    virtual ~ERException() throw(){};

    virtual const char* what() const throw();

    const char* msg() const throw();
    
  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

