#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include "LArUtil/GeometryUtilities.h"
#include "LArUtil/DetectorProperties.h"


namespace flashana {

  PhotonLibHypothesis::PhotonLibHypothesis(const std::string name)
    : BaseFlashHypothesis(name)
  {}

  void PhotonLibHypothesis::Configure(const ::fcllite::PSet &pset)
  {}
  
  void PhotonLibHypothesis::FillEstimate(const QCluster_t& trk,
					 Flash_t &flash) const
  {
    

    size_t n_pmt = BaseAlgorithm::NOpDets();//n_pmt returns 0 now, needs to be fixed
    
    double t2cm   = larutil::GeometryUtilities::GetME()->TimeToCm();
    double ROrate = larutil::DetectorProperties::GetME()->SamplingRate(); // ns
    const double det_width = 256.35; // cm

    for ( auto& v : flash.pe_v ) v = 0;
    bool oob(false);

    double clus_x_min =  1036.; // cm
    double clus_x_max =  -1036.; // cm
    double clus_tdrift_min = 1E9; //nsec, independent of translation of track's x
    for (auto const& pt : trk){
      if (pt.x < clus_x_min) { clus_x_min = pt.x; }
      if (pt.x > clus_x_max) { clus_x_max = pt.x; }
      if (pt.t < clus_tdrift_min) { clus_tdrift_min = pt.t; }
    }

    // The earliest part of the track can not arrive after the upper bound measured drift time if it is to belong to this flash.
    double clus_t_min = (clus_x_min/t2cm)*(ROrate/1000.); // us
    if ( clus_t_min - clus_tdrift_min/1000. >  flash.time 
	 ||  clus_x_max > det_width
	 ||  clus_x_min < 0.0
	 )  // us
      oob = true;

    if (oob) 
      std::cout << "PLH: out of bounds. clus_t_min, clus_tdrift_min: " << clus_t_min <<", " << clus_tdrift_min/1000.  << std::endl;

    for ( size_t ipmt = 0; ipmt < 32 && !oob ; ++ipmt) {

      std::vector<double> pt_last;
      for ( size_t ipt = 0; ipt < trk.size(); ++ipt) {
	
        auto const& pt = trk[ipt];
	
	double dist(0.0);
	if (pt_last.size() == 3 )
	  dist = std::sqrt(std::pow(pt_last.at(0)-pt.x,2.)+std::pow(pt_last.at(1)-pt.y,2.)+std::pow(pt_last.at(2)-pt.z,2.));
	pt_last.clear();
	pt_last.push_back(pt.x); pt_last.push_back(pt.y); pt_last.push_back(pt.z);

        double q = pt.q ;
	double q_corr = dist*2.2*21000 *0.7; // cm * MeV/cm * photons/MeV * quenching(Efield)

	//		std::cout << "PhotonLibHypothesis: PMT : " << ipmt << " [x,y,z,t,q] -> [q,q_corr] : [" << pt.x << ", "
	//	  << pt.y << ", " << pt.z << ", , " << pt.t<< ", " << pt.q <<"] -> [" << q << ", " << q_corr << "]" << std::endl;
	//std::cout << "PhotonLibHypothesis: PMT : dist to last point is " <<  dist << std::endl;

        q *= ::phot::PhotonVisibilityService::GetME().GetVisibility( pt.x, pt.y, pt.z, ipmt)*0.23*0.0093; //Should be 0.23. //EC, 16-Dec-2015 
	//*0.0093;
        flash.pe_v[ipmt] += q;
	
	//			std::cout << "PhotonLibHypothesis: PMT : " << ipmt << " [x,y,z,q] -> [q] : [" << pt.x << ", "
	//		  << pt.y << ", " << pt.z << ", " << pt.q <<"] -> [" << q << "]" << std::endl;

      }

    }

    // zero the flash if the track was translated till it stepped out of TPC 
    if (oob)   for ( auto& v : flash.pe_v ) v = 0; 

    return;
  }
}
#endif
