/**
 * \file endpoint2d.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for endpoint2d data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_ENDPOINT2D_H
#define LARLITE_ENDPOINT2D_H

#include "data_base.h"

namespace larlite {
  /**
     \class endpoint2d
     LArSoft EndPoint2D class equivalent data container
  */
  class endpoint2d : public data_base {
    
  public:
    
    /// Default constructor
    endpoint2d() : data_base(data::kEndPoint2D) {clear_data();}

    /// Alternative constructor
    endpoint2d(Double_t    driftTime,
	       UInt_t      wire,
	       Double_t    strength,
	       Int_t       id,
	       geo::View_t view,
	       Double_t    totalQ);
    
    /// Default destructor
    virtual ~endpoint2d(){}

    /// Clear method
    virtual void clear_data();

    ///--- Getters ---///
    Double_t    Charge()    const { return fTotalCharge; }
    geo::View_t View()      const { return fView;        }
    Double_t    DriftTime() const { return fDriftTime;   }
    UInt_t      Wire()      const { return fWire;        }
    Int_t       ID()        const { return fID;          }
    Double_t    Strength()  const { return fStrength;    }

  private:

    Double_t    fDriftTime;    ///< vertex's drift time
    UInt_t      fWire;         ///< vertex's wireID
    Int_t       fID;           ///< vertex's ID
    Double_t    fStrength;     ///< vertex's strength
    geo::View_t fView;         ///< view for this end point
    Double_t    fTotalCharge;  ///< total charge of hits associated with end point 
    
    ////////////////////////
    ClassDef(endpoint2d,1)
    ////////////////////////
      
  };
  
  /**
     \class event_endpoint2d
     A collection storage class of multiple endpoint2d points
  */
  class event_endpoint2d : public std::vector<larlite::endpoint2d>,
			   public event_base {
    
  public:
    
    /// Default constructor
    event_endpoint2d(std::string name="noname") : event_base(data::kEndPoint2D,name) {clear_data();}
    
    /// Default copy constructor
    event_endpoint2d(const event_endpoint2d& original) : std::vector<larlite::endpoint2d>(original),
							 event_base(original)
    {}
    
    /// Default destructor
    ~event_endpoint2d(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_endpoint2d,1)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
