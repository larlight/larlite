/**
 * \file cosmictag.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for cosmictag data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_COSMICTAG_H
#define LARLITE_COSMICTAG_H

#include <vector>
#include <iosfwd>
#include <iostream>
#include <iomanip>
#include "Base/AnalysisConstants.h"
#include "data_base.h"

namespace larlite {

  /**
     \class cosmictag
     LArSoft CosmicTag class equivalent data container
  */
  class cosmictag : public data_base {

  public:

    /// Default ctor
    cosmictag();
    /// Default dtor
    ~cosmictag(){}

    std::vector<float> endPt1; ///< x,y,z assuming t_0 = t_beam
    std::vector<float> endPt2; ///< x,y,z assuming t_0 = t_beam
    /**
       -1 means very likely neutrino, 
       0 means probably not a cosmic (or unknown), 
       1 means cosmic
    */
    float fCosmicScore; 
    /// Refer to anab::CosmicTagID_t
    anab::CosmicTagID_t fCosmicType; 

  public:

    /// Alternative ctor
    cosmictag(std::vector<float> ePt1,
	      std::vector<float> ePt2,
	      float cScore,
	      anab::CosmicTagID_t cTypes);

    /// Alternative ctor
    cosmictag(float cScore);

    /// Streamer
    friend std::ostream& operator << (std::ostream &o, cosmictag const& a);

    float getXInteraction(float oldX, float xDrift, int tSample,  
			  float realTime, int tick ); 

    inline const float& CosmicScore() const {return fCosmicScore; }

    inline const anab::CosmicTagID_t& CosmicType() const {return fCosmicType; }

  protected:
    
  };

  /**
     \class event_cosmictag
     Collection of cosmictag objects to be stored per event
  */
  class event_cosmictag : public std::vector<larlite::cosmictag>,
			  public event_base {
    
  public:

    /// Default constructor
    event_cosmictag(std::string name="noname") : event_base(data::kCosmicTag,name) { clear_data();}
    
    /// Default copy constructor
    event_cosmictag(const event_cosmictag& original) : std::vector<larlite::cosmictag>(original),
						       event_base(original)
    {}
    
    /// Default destructor
    ~event_cosmictag(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:

  };

}

#endif //ANAB_COSMICTAG
