/**
 * \file simch.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for simch data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat
    
@{*/

#ifndef SIMCH_H
#define SIMCH_H

#include "data_base.h"
#include <map>
namespace larlite {

  /**
     \class ide
     LArSoft sim::IDE equivalent data product
  */
  class ide : public TObject {
    
  public:

    /// Default ctor
    ide() : TObject() {clear_data();}

    /// Default dtor
    ~ide(){}

    /// Copy ctor
    ide(const ide& origin) : TObject(origin),
			     trackID(origin.trackID),
			     numElectrons(origin.numElectrons),
			     energy(origin.energy),
			     x(origin.x),
			     y(origin.y),
			     z(origin.z)
    {}

    Int_t    trackID;      ///< Geant4 supplied track ID
    Double_t numElectrons; ///< total number of electrons for this track ID and time
    Double_t energy;       ///< total energy deposited for this track ID and time
    Double_t x;            ///< x position of ionization
    Double_t y;            ///< y position of ionization
    Double_t z;            ///< z position of ionization
    
    /// Initialize attributes
    void clear_data();

  private:

    ////////////////////////
    ClassDef(ide,1)
    ////////////////////////
      
  };

  /**
     \class simch
     LArSoft sim::SimChannel equivalent data product
  */
  class simch : public data_base {
    
  public:

    /// Default constructor
    simch() : data_base(data::kSimChannel) {clear_data();}

    /// Default destructor
    virtual ~simch(){}

    /// Clear method
    virtual void clear_data();

    ///--- Setters ---///
    void set_channel(UShort_t ch)
    { fChannel = ch; }

    void add_ide(UShort_t tdc, ide in);

    ///--- Getters ---///

    /// Channel number getter
    UShort_t Channel() const
    { return fChannel; }

    /**
       method to return a collection of IDE structs for all geant4
       track ids represented between startTDC and endTDC
    */
    std::vector<larlite::ide> TrackIDsAndEnergies(UShort_t startTDC,
						   UShort_t endTDC) const;
    
    /// IDE map getter
    const std::map<UShort_t, std::vector<larlite::ide> >& TDCIDEMap() const
    { return fTDCIDEs; }

    /**
       The number of ionization electrons associated with this channel for the
       specified TDC.
    */
    Double_t Charge(UShort_t tdc) const;

    /**
       Total energy deposit associated with this channel for the specified TDC.
    */    
    Double_t Energy(UShort_t tdc) const;

    /// Comparison operator
    //inline bool operator< (const simch& other) { return fChannel < other.fChannel };

  private:

    /// electronics channel associated with these sim::Electrons
    UShort_t fChannel; 

    /// vector of IDE structs for each TDC with signal
    std::map<UShort_t, std::vector< larlite::ide > > fTDCIDEs;

    ////////////////////////
    ClassDef(simch,1)
    ////////////////////////
      
  };

  /**
     \class event_simch
     A collection storage class of multiple simch
  */
  class event_simch : public std::vector<larlite::simch>,
		      public event_base {
    
  public:
    
    /// Default constructor
    event_simch(std::string name="noname") : event_base(data::kSimChannel,name) {clear_data();}
    
    /// Default copy constructor
    event_simch(const event_simch& original) : std::vector<larlite::simch>(original),
					       event_base(original)
    {}
    
    /// Default destructor
    ~event_simch(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    //////////////////////////
    ClassDef(event_simch,1)
    //////////////////////////
  };
  
}
#endif

/** @} */ // end of doxygen group 
