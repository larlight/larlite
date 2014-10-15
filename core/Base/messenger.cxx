#ifndef MESSENGER_CXX
#define MESSENGER_CXX

#include "messenger.h"
namespace larlite {
  Message* Message::me = 0;
  
  void Message::send(msg::Level level, std::string msg)
  {
    std::cout 
      << msg::ColorPrefix[level].c_str()
      << msg::StringPrefix[level].c_str()
      << "\033[0m"
      << msg.c_str()
      << std::endl;
  }
  
  void Message::send(msg::Level level, std::string where, std::string msg)
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
#endif
