#ifndef ERTOOL_EREXCEPTION_CXX
#define ERTOOL_EREXCEPTION_CXX

#include "ERException.h"
#include "StackTrace.h"
namespace ertool {

  ERException::ERException(const std::string& msg)
    : std::exception()
    {
      print_stacktrace();
      _msg = "\033[93m EXCEPTION \033[00m\033[95m";
      _msg += msg;
      _msg += "\033[00m\n";
    }

  const char* ERException::what() const throw()
  { return _msg.c_str(); }

}
#endif
