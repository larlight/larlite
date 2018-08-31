/**
 * \file larflow3dhit.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for larflow3dhit data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_LARFLOW3DHIT_H
#define LARLITE_LARFLOW3DHIT_H

#include "data_base.h"
#include "Base/GeoConstants.h"
#include "Base/GeoTypes.h"
#include "Base/RawConstants.h"
#include <vector>
namespace larlite{
  /**
     \class larflow3dhit
     LArSoft Larflow3dhit class equivalent data container
  */
  class larflow3dhit : public data_base {
    
  public:
    
    /// Default constructor
    larflow3dhit();
    
    /// Default destructor
    virtual ~larflow3dhit(){}

    void clear_data();

    // Set methods

    /// @{
    /// @name Accessors
      
      
    /// @}
      
    
  protected:

  private:
    
  };
  
  /**
     \class event_hit
     A collection storage class of multiple hits.
  */
  class event_larflow3dhit : public std::vector<larlite::larflow3dhit>, public event_base {
  public:
      
    /// Default constructor
    event_larflow3dhit(std::string name="noname") : event_base(data::kLArFlow3DHit,name) { clear_data(); }
    
    
    /// Default copy constructor
    event_larflow3dhit(const event_larflow3dhit& original) : std::vector<larlite::larflow3dhit>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_larflow3dhit(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
