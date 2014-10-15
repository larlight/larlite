////////////////////////////////////////////////////////////////////////////
// \version 0.0
//
// \brief Definition of data product to hold CosmicTag information
//
// \author lockwitz@fnal.gov
//
////////////////////////////////////////////////////////////////////////////
#ifndef ANAB_COSMICTAG_H
#define ANAB_COSMICTAG_H

#include <vector>
#include <iosfwd>
#include <iostream>
#include <iomanip>



namespace anab {

  typedef enum cosmic_tag_id{
    kUnknown=-1,
    kNotTagged=0,
    kGeometry_YY=1,
    kGeometry_YZ,
    kGeometry_ZZ,
    kGeometry_XX,
    kGeometry_XY,
    kGeometry_XZ,
    kGeometry_Y=21,
    kGeometry_Z,
    kGeometry_X,
    kOutsideDrift_Partial=100,
    kOutsideDrift_Complete,
    kFlash_BeamIncompatible=200,
    kFlash_Match=300
  } CosmicTagID_t;

  class CosmicTag{
  public:
    
    CosmicTag();

    std::vector<float> endPt1; // x,y,z assuming t_0 = t_beam
    std::vector<float> endPt2; // x,y,z assuming t_0 = t_beam
    float fCosmicScore; // -1 means very likely neutrino, 
                        // 0 means probably not a cosmic (or unknown), 
                        // 1 means cosmic
    CosmicTagID_t fCosmicType; 

#ifndef __GCCXML__
  public:

    CosmicTag(std::vector<float> ePt1,
	      std::vector<float> ePt2,
	      float cScore,
	      CosmicTagID_t cTypes);

    CosmicTag(float cScore);



    friend std::ostream& operator << (std::ostream &o, CosmicTag const& a);

    float getXInteraction(float oldX, float xDrift, int tSample,  
			  float realTime, int tick ); 

    const float& CosmicScore() const;
    const CosmicTagID_t& CosmicType() const;
    
#endif
  };

}

#ifndef __GCCXML__

inline const float& anab::CosmicTag::CosmicScore() const {return fCosmicScore; }
inline const anab::CosmicTagID_t& anab::CosmicTag::CosmicType() const {return fCosmicType; }


#endif

#endif //ANAB_COSMICTAG
