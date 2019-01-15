/**
 * \file larlite_base.h
 *
 * \ingroup Base
 * 
 * \brief Class definition file of larlite_base
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_BASE_H
#define LARLITE_BASE_H

#include <vector>
#include "FrameworkConstants.h"
#include "messenger.h"

namespace larlite{
  
  /**
     \class larlite_base
     Very base class of all everything else defined in this framework.
  */
  class larlite_base {
    
  public:
    
    /// Default constructor
    larlite_base() : _verbosity(msg::kMSG_TYPE_MAX,false), _name("larlite_base")
    {set_verbosity(msg::kNORMAL);}
    
    /// Default copy constructor
    larlite_base(const larlite_base &original) : _name(original._name){};
    
    /// Default destructor
    virtual ~larlite_base(){};
    
    /// Setter for the verbosity level 
    virtual void set_verbosity(msg::Level level);
    
    /// Getter for the verbosity level
    inline msg::Level get_verbosity() const {return _verbosity_level;};
    
    /// Getter for the class name
    inline const std::string class_name() const {return _name;};
    
    /// message print out method
    inline void print(msg::Level level, std::string where, std::string msg) const
    {if(_verbosity.at(level)) Message::send(level,where,msg);};
    
    /// message print out method
    inline void print(msg::Level level, std::string msg) const
    {if(_verbosity.at(level)) Message::send(level,msg);};
    
  protected:

    char _buf[5120];                ///< char buffer for message manipulation
    std::vector<bool> _verbosity;  ///< holder for enabled message levels
    msg::Level _verbosity_level;   ///< holder for specified verbosity level
    std::string _name;             ///< class name holder
    
  };
}
#endif

/** @} */ // end of doxygen group
