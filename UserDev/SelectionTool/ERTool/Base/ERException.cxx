#ifndef ERTOOL_EREXCEPTION_CXX
#define ERTOOL_EREXCEPTION_CXX

#include "ERException.h"
#include "StackTrace.h"
namespace ertool {

ERException::ERException(const std::string& msg)
  : std::exception()
{
  _msg = "\033[93m EXCEPTION \033[00m\033[95m";
  _msg += msg;
  _msg += "\033[00m\n";
}

const char* ERException::what() const throw()
{ 
  print_stacktrace();
  return _msg.c_str();
}

const char* ERException::msg() const throw()
{
  return _msg.c_str();
}

}
#endif
