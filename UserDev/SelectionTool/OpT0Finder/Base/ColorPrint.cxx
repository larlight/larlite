#ifndef OPT0FINDER_COLORPRINT_CXX
#define OPT0FINDER_COLORPRINT_CXX

#include "ColorPrint.h"
#include "OpT0FinderException.h"
namespace flashana {

  void ColorPrint::Print(msg::MSGLevel_t level, const std::string& msg) const
  {
    if(level == msg::kEXCEPTION) {
      throw OpT0FinderException(msg);
    }
    else if(level < _verbosity) return;
    else{
      std::cout 
	<< msg::kColorPrefix[level].c_str()
	<< msg::kStringPrefix[level].c_str()
	<< "\033[0m"
	<< msg.c_str()
	<< std::endl;
    }
  }

  void ColorPrint::Print(msg::MSGLevel_t level, const std::string& header, const std::string& msg) const
  {
    if(level == msg::kEXCEPTION) {
      std::string exception_msg = "\033[95m<" + header + ">\033[0m " + msg;
      throw OpT0FinderException(exception_msg);
    }
    else if(level < _verbosity) return;
    else{
      std::cout 
	<< msg::kColorPrefix[level].c_str()
	<< msg::kStringPrefix[level].c_str()
	<< "\033[0m"
	<< "\033[95m"
	<< "<"
	<< header.c_str()
	<< "> "
	<< "\033[0m"
	<< msg.c_str()
	<< std::endl;
    }
  }

}
#endif
