#ifndef SELECTIONTOOL_ERTOOL_MESSENGER_CXX
#define SELECTIONTOOL_ERTOOL_MESSENGER_CXX

#include "Message.h"
namespace ertool {
  namespace msg {

    Message* Message::me = 0;
    
    void Message::send_msg(Level_t level, const std::string& msg)
    {
      std::cout 
	<< msg::ColorPrefix[level].c_str()
	<< msg::StringPrefix[level].c_str()
	<< "\033[0m"
	<< msg.c_str()
	<< std::endl;
    }
    
    void Message::send_msg(Level_t level, const std::string& where, const std::string& msg)
    {
      std::cout 
	<< msg::ColorPrefix[level].c_str()
	<< msg::StringPrefix[level].c_str()
	<< "\033[0m"
	<< "\033[95m"
	<< "<"
	<< where.c_str()
	<< "> "
	<< "\033[0m"
	<< msg.c_str()
	<< std::endl;
    }
  }
}
#endif
