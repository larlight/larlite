////////////////////////////////////////////////////////////////////////
// \version $Id: 
//
// \brief Definition of CosmicTag analysis object
//
// \author lockwitz@fnal.gov
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_COSMICTAG_CXX
#define LARLITE_COSMICTAG_CXX

//#include "AnalysisBase/CosmicTag.h"
//#include "messagefacility/MessageLogger/MessageLogger.h"
#include "cosmictag.h"

namespace larlite{

  //----------------------------------------------------------------------
  cosmictag::cosmictag() :
    data_base(data::kCosmicTag),
    endPt1(0),
    endPt2(0),
    fCosmicScore(0),
    fCosmicType(anab::CosmicTagID_t::kUnknown)
  {
  }

  //----------------------------------------------------------------------
  cosmictag::cosmictag(float f) :
    data_base(data::kCosmicTag),
    endPt1(0),
    endPt2(0),
    fCosmicScore(f),
    fCosmicType(anab::CosmicTagID_t::kUnknown)
  {
  }

  //----------------------------------------------------------------------
  cosmictag::cosmictag(std::vector<float> ePt1,
		       std::vector<float> ePt2,
		       //		       double flashTime, // should go
		       float cScore,
		       anab::CosmicTagID_t cType) :
    data_base(data::kCosmicTag),
    endPt1(ePt1),
    endPt2(ePt2),
    fCosmicScore(cScore),
    fCosmicType(cType)
  {
  }
  

  
  //----------------------------------------------------------------------
  // ostream operator.  
  //
  std::ostream& operator<< (std::ostream & o, cosmictag const& a)
  {
    o //<< "Flash Time     : "     <<  a.flashTime     // should go
       << "\n Cosmic Score     : "  <<  a.fCosmicScore     
       << "\n Cosmic Type     : "  <<  a.fCosmicType    
       << "\n End Point 1: " <<std::endl;
     for(size_t i=0;i<a.endPt1.size(); i++)  
       o << a.endPt1[i] << ", ";
     o << std::endl;
     o << "\n End Point 2: " <<std::endl;
     for(size_t i=0;i<a.endPt2.size(); i++)  
       o << a.endPt2[i] << ", ";
     o << std::endl;


    return o;
  }
 
  float cosmictag::getXInteraction(float oldX, float xDrift, int tSample, 
				   float realTime, int tick ) {
    // fix this
    int t =3*tSample;
    if( (tick > 0*tSample) && (tick < 1*tSample) ) t = 0*tSample;
    if( (tick > 1*tSample) && (tick < 2*tSample) ) t = 1*tSample;
    if( (tick > 2*tSample) && (tick < 3*tSample) ) t = 2*tSample; 

    float newX = oldX + (xDrift/tSample)*(t - realTime);
    return newX;
  }
  
  
}

#endif
