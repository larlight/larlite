////////////////////////////////////////////////////////////////////////////
// \version 0.0
//
// \brief Definition of data product to hold CosmicTag information
//
// \author lockwitz@fnal.gov
//
////////////////////////////////////////////////////////////////////////////
#ifndef LARLITE_COSMICTAG_H
#define LARLITE_COSMICTAG_H

#include <vector>
#include <iosfwd>
#include <iostream>
#include <iomanip>
#include "Base/AnalysisConstants.h"
#include "data_base.h"

namespace larlite {

  class cosmictag : public data_base {

  public:
    
    cosmictag();

    std::vector<float> endPt1; // x,y,z assuming t_0 = t_beam
    std::vector<float> endPt2; // x,y,z assuming t_0 = t_beam
    float fCosmicScore; // -1 means very likely neutrino, 
                        // 0 means probably not a cosmic (or unknown), 
                        // 1 means cosmic
    anab::CosmicTagID_t fCosmicType; 

  public:

    cosmictag(std::vector<float> ePt1,
	      std::vector<float> ePt2,
	      float cScore,
	      anab::CosmicTagID_t cTypes);

    cosmictag(float cScore);



    friend std::ostream& operator << (std::ostream &o, cosmictag const& a);

    float getXInteraction(float oldX, float xDrift, int tSample,  
			  float realTime, int tick ); 

    inline const float& CosmicScore() const {return fCosmicScore; }

    inline const anab::CosmicTagID_t& CosmicType() const {return fCosmicType; }

  protected:
    
    ClassDef(cosmictag,1)

  };

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

    ClassDef(event_cosmictag,1);

  };

}

#endif //ANAB_COSMICTAG
