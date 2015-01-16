#ifndef LARLITE_PXHITCONVERTER_CXX
#define LARLITE_PXHITCONVERTER_CXX

#include "PxHitConverter.h"

namespace larutil {

  
   /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
  void PxHitConverter::GeneratePxHit(const std::vector< ::larlite::hit > hits,
				std::vector<larutil::PxHit> &pxhits) const
  {

    if(!(hits.size())) throw LArUtilException(Form("Hit list empty! (%s)",__FUNCTION__));
    
    std::vector<unsigned int> hit_index;
    hit_index.clear();
    hit_index.reserve(hits.size());
    
    
    //generate full index
    for(unsigned int ix=0; ix<hits.size();++ix ) {
      
      hit_index.at(ix)=ix;
      
    }      

    GeneratePxHit(hit_index,hits,pxhits);
    
  }
  
  
  
   void PxHitConverter::GenerateSinglePxHit(const ::larlite::hit &hit,
		             larutil::PxHit &pxhit) const
   {
	
    auto geo  = ::larutil::Geometry::GetME();
    auto geou = ::larutil::GeometryUtilities::GetME();
    //util::GeometryUtilities  gser;
         
    UChar_t plane = geo->ChannelToPlane(hit.Channel());
	 
    pxhit.t = hit.PeakTime() * geou->TimeToCm();
    pxhit.w = hit.Wire()     * geou->WireToCm();

    pxhit.charge = hit.Charge();
    pxhit.peak   = hit.Charge(true);
    pxhit.plane  = plane;	
    
    
			       
   }
		       
		       
    
  /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
  void PxHitConverter::GeneratePxHit(const std::vector<unsigned int>& hit_index,
				std::vector< ::larlite::hit> hits,
				std::vector<larutil::PxHit> &pxhits) const
  {

    if(!(hit_index.size())) throw LArUtilException(Form("Hit list empty! (%s)",__FUNCTION__));
    
    pxhits.clear();
    pxhits.reserve(hit_index.size());


    for(auto const& index : hit_index) {
      
      auto const& hit = hits[index];

      PxHit h;
      GenerateSinglePxHit(hit,h);

      pxhits.push_back(h);
    }      

  }
  
  
  
}// end namespace util

#endif