/**
 * \file trackmom.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for trackmom data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_TRACKMOM_H
#define LARLITE_TRACKMOM_H

#include "data_base.h"
#include "Base/GeoTypes.h"
#include "Base/RecoTypes.h"
#include <TVector3.h>
namespace larlite{
  /**
     \class trackmom
     LArSoft trackmom class equivalent data container
  */
  class trackmom : public data_base {
    
  public:
    
    /// Default constructor
    trackmom();

    /// Copy constructor
    trackmom(const trackmom& original);
    
    /// Default destructor
    virtual ~trackmom(){}

    virtual void clear_data();

    // Set methods
    void Momentum(const data::TrackMomentumType_t type,
		  const double mom);

    // Get Methods
    double Momentum(const data::TrackMomentumType_t type);

  protected:

    std::vector<double> _momentum; ///< Various momentum representation
    
  };
  
  /**
     \class event_trackmom
     A collection storage class of multiple trackmoms.
  */
  class event_trackmom : public std::vector<larlite::trackmom>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_trackmom(std::string name="noname") : event_base(data::kTrackMomentum,name) {clear_data();}
    
    /// Default copy constructor
    event_trackmom(const event_trackmom& original) : std::vector<larlite::trackmom>(original),
							   event_base(original)
    {}
    
    /// Default destructor
    ~event_trackmom(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
