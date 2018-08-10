/**
 * \file RecoEmulatorException.h
 *
 * \ingroup RecoEmulator
 *
 * \brief Class def header for exception classes in RecoEmulator
 *
 * @author kaleko
 */

/** \addtogroup RecoEmulator

    @{*/
#ifndef RECOEMULATOR_RECOEMULATOREXCEPTION_H
#define RECOEMULATOR_RECOEMULATOREXCEPTION_H

#include <iostream>
#include <exception>

namespace recoemu {
  /**
     \class RecoEmulatorException
     Generic (base) exception class
  */
  class RecoEmulatorException : public std::exception{

  public:

    RecoEmulatorException(const std::string& msg = "");

    virtual ~RecoEmulatorException() throw() {};

    virtual const char* what() const throw();

    const char* msg() const throw();

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group

