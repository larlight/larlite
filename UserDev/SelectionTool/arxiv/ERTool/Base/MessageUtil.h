/**
 * \file Message.h
 *
 * \ingroup Base
 * 
 * \brief Message utility class definition header file.
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef SELECTIONTOOL_ERTOOL_MESSAGE_H
#define SELECTIONTOOL_ERTOOL_MESSAGE_H

#include <cstdio>
#include <iostream>
#include "ERToolTypes.h"
namespace ertool {
  /**
     \class Message
     \brief Utility class used to show formatted message on the screen.
  */
  class MessageUtil{
    
  public:
    
    /// Default constructor ... make it private to limit the duplication of instance
    MessageUtil();
    
    /// Default destructor
    virtual ~MessageUtil(){}

    const msg::Level_t Verbosity() const { return _verbosity; }

    void SetVerbosity(const msg::Level_t verbosity) { _verbosity = verbosity; }

    inline bool Debug   () const { return _verbosity <= msg::kDEBUG;   }
    inline bool Info    () const { return _verbosity <= msg::kINFO;    }
    inline bool Normal  () const { return _verbosity <= msg::kNORMAL;  }
    inline bool Warning () const { return _verbosity <= msg::kWARNING; }
    inline bool Error   () const { return _verbosity <= msg::kERROR;   }

    void Debug     (const std::string& msg) const;
    void Info      (const std::string& msg) const;
    void Normal    (const std::string& msg) const;
    void Warning   (const std::string& msg) const;
    void Error     (const std::string& msg) const;
    void Exception (const std::string& msg) const;

    void Debug     (const std::string& where, const std::string& msg) const;
    void Info      (const std::string& where, const std::string& msg) const;
    void Normal    (const std::string& where, const std::string& msg) const;
    void Warning   (const std::string& where, const std::string& msg) const;
    void Error     (const std::string& where, const std::string& msg) const;
    void Exception (const std::string& where, const std::string& msg) const;

  private:

    msg::Level_t _verbosity;
    void Send(msg::Level_t level, const std::string& msg) const;
    void Send(msg::Level_t level, const std::string& where, const std::string& msg) const;
  };
}
#endif
  
/** @} */ // end of doxygen group Message
