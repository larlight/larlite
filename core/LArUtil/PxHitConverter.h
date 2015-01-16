#ifndef LARUTIL_PXHITCONVERTER_H
#define LARUTIL_PXHITCONVERTER_H

//#include <TMath.h>
//#include <TLorentzVector.h>

#include "PxUtils.h"
#include <iostream>
#include "DataFormat/storage_manager.h"
#include "GeometryUtilities.h"
//#include <climits>
#include <iostream>
#include <vector>
#include "LArUtilException.h"


///General LArSoft Utilities
namespace larutil{

  
  //class GeometryUtilities : public larlight::larlight_base {
  class PxHitConverter {

  public:

      public:
    
    /// Default constructor
    PxHitConverter(){};
    
    /// Default destructor
    virtual ~PxHitConverter(){};

//     /// Generate: from 1 set of hits => 1 CPAN using indexes (association)
//     void GenerateCPAN(const std::vector<unsigned int>& hit_index,
// 		      const larlite::event_hit* hits,
// 		      ClusterParamsAlg &cpan) const;
//     
//     /// Generate: CPAN vector from event storage by specifying cluster type
//     void GenerateCPAN(::larlite::storage_manager* storage,
// 		      const std::string &cluster_producer_name,
// 		      std::vector<cluster::ClusterParamsAlg> &cpan_v) const;
    
    /// Generate: from 1 set of hits => 1 set of PxHits using indexes (association)
   void GeneratePxHit(const std::vector<unsigned int>& hit_index,
				std::vector< ::larlite::hit> hits,
				std::vector<larutil::PxHit> &pxhits) const;

      /// Generate: from 1 set of hits => 1 set of PxHits using using all hits
   void GenerateSinglePxHit(const ::larlite::hit &hit,
		             larutil::PxHit &pxhit) const;    
		       
   void GeneratePxHit(const std::vector< ::larlite::hit> hits,
				std::vector<larutil::PxHit> &pxhits) const;
		       

			     
		       
//     /// Generate: vector of PxHit sets from event storage by specifying cluster type
//     void GeneratePxHit(larlite::storage_manager* storage,
// 		       const std::string &cluster_producer_name,
// 		       std::vector<std::vector<larutil::PxHit> > &pxhits_v) const;
    
    }; // class PxHitConverter

} //namespace utils
#endif // UTIL_PXHITCONVERTER_H
