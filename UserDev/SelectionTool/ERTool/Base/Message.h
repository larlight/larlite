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
#include "ERToolConstants.h"
namespace ertool {
  namespace msg {
    /**
       \class Message
       \brief Utility class used to show formatted message on the screen.
    */
    class Message{
      
    private:
      
      /// Default constructor ... make it private to limit the duplication of instance
      Message(){};
      
      /// Default destructor
      virtual ~Message(){};
      
      /// Private static pointer
      static Message* me;
      
    public:
      
      /// Getter of a message instance 
      /** This should not be needed normally because a method to print out a message
	  is defined as a static method. See Message::send
      */
      static Message* get()
      {
	if(!me) me=new Message();
	return me;
      };
      
      /// Static method to send message out.
      static void send(Level_t level, std::string msg);
      
      /// Extra argument "where" is used to indicate function/class name.
      static void send(Level_t level, std::string where, std::string msg);
      
    };
  }
}
#endif
  
/** @} */ // end of doxygen group Message
