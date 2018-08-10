#ifndef RECOEMULATOR_RECOEMULATOREXCEPTION_CXX
#define RECOEMULATOR_RECOEMULATOREXCEPTION_CXX

#include "RecoEmulatorException.h"

namespace recoemu {

	RecoEmulatorException::RecoEmulatorException(const std::string& msg)
		: std::exception()
	{
		_msg = "\033[93m EXCEPTION \033[00m\033[95m";
		_msg += msg;
		_msg += "\033[00m\n";
	}

	const char* RecoEmulatorException::what() const throw()
	{
		return _msg.c_str();
	}

	const char* RecoEmulatorException::msg() const throw()
	{
		return _msg.c_str();
	}

}
#endif
