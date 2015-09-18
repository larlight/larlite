#ifndef SELECTIONTOOL_ERTOOL_MESSENGER_CXX
#define SELECTIONTOOL_ERTOOL_MESSENGER_CXX

#include "MessageUtil.h"
#include "ERException.h"
#include "ERToolConstants.h"
namespace ertool {

  MessageUtil::MessageUtil()
    : _verbosity(msg::kNORMAL)
  {}
  
  void MessageUtil::Send(msg::Level_t level, const std::string& msg) const
  {
    if(level == msg::kEXCEPTION)
      throw ERException(msg);

    if(level > _verbosity) return;

    std::cout
      << msg::ColorPrefix[level].c_str()
      << msg::StringPrefix[level].c_str()
      << "\033[0m"
      << msg.c_str()
      << std::endl;
  }
  
  void MessageUtil::Send(msg::Level_t level, const std::string& where, const std::string& msg) const
  {
    if(level == msg::kEXCEPTION) {
      std::string txt;
      txt += "<" + where + "> " + msg;
      throw ERException(txt);
    }

    if(level > _verbosity) return;
    
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

  void MessageUtil::Debug     (const std::string& msg) const
  { Send(msg::kDEBUG,msg);     }
  void MessageUtil::Info      (const std::string& msg) const
  { Send(msg::kINFO,msg);      }
  void MessageUtil::Normal    (const std::string& msg) const
  { Send(msg::kNORMAL,msg);    }
  void MessageUtil::Warning   (const std::string& msg) const
  { Send(msg::kWARNING,msg);   }
  void MessageUtil::Error     (const std::string& msg) const
  { Send(msg::kERROR,msg);     }
  void MessageUtil::Exception (const std::string& msg) const
  { Send(msg::kEXCEPTION,msg); }
  
  void MessageUtil::Debug     (const std::string& where, const std::string& msg) const
  { Send(msg::kDEBUG,where,msg);     }
  void MessageUtil::Info      (const std::string& where, const std::string& msg) const
  { Send(msg::kINFO,where,msg);      }
  void MessageUtil::Normal    (const std::string& where, const std::string& msg) const
  { Send(msg::kNORMAL,where,msg);    }
  void MessageUtil::Warning   (const std::string& where, const std::string& msg) const
  { Send(msg::kWARNING,where,msg);   }
  void MessageUtil::Error     (const std::string& where, const std::string& msg) const
  { Send(msg::kERROR,where,msg);     }
  void MessageUtil::Exception (const std::string& where, const std::string& msg) const
  { Send(msg::kEXCEPTION,where,msg); }
  
}
#endif
