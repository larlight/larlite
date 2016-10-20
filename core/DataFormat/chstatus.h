/**
 * \file chstatus.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for chstatus data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_CHSTATUS_H
#define LARLITE_CHSTATUS_H

#include <vector>
#include "Base/AnalysisConstants.h"
#include "Base/GeoTypes.h"
#include "Base/DetectorStatusTypes.h"
#include "data_base.h"

namespace larlite {

  /**
     \class chstatus
     LArSoft Chstatus class equivalent data container
  */
  class chstatus : public data_base {

  public:

    /// Default ctor
    chstatus();

    /// Default dtor
    ~chstatus(){}

    void set_status(const geo::PlaneID& plane, const std::vector<short>& status_v);

#ifndef __CINT__
    void set_status(const geo::PlaneID& plane, std::vector<short>&& status_v);
#endif

    const std::vector<short>& status() const { return fStatus_v; }

    const geo::PlaneID& plane() const { return fPlaneID; }

  private:

    geo::PlaneID fPlaneID;           ///< PlaneID
    std::vector<short> fStatus_v;    ///< Status vector
    
  };

  /**
     \class event_chstatus
     Collection of chstatus objects to be stored per event
  */
  class event_chstatus : public std::vector<larlite::chstatus>,
    public event_base {
      
  public:
      
      /// Default constructor
      event_chstatus(std::string name="noname");
      
    /// Default copy constructor
    event_chstatus(const event_chstatus& original) : std::vector<larlite::chstatus>(original),
      event_base(original)
    {}
    
    /// Default destructor
    ~event_chstatus(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:

  };

}

#endif //ANAB_CHSTATUS
