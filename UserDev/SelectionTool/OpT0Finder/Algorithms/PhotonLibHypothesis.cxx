#ifndef PHOTONLIBHYPOTHESIS_CXX
#define PHOTONLIBHYPOTHESIS_CXX

#include "PhotonLibHypothesis.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"


namespace flashana {

  PhotonLibHypothesis::PhotonLibHypothesis(const std::string name)
    : BaseFlashHypothesis(name)
  {
    _n_pmt = BaseAlgorithm::NOpDets();//n_pmt returns 0 now, needs to be fixed. EC 31-Mar-2016: Still true?
    std::cout << "PhotonLibHyp: Constructor: _n_pmt is " << _n_pmt << std::endl;
  }

  void PhotonLibHypothesis::Configure(const ::fcllite::PSet &pset)
  {
  }
  
  void PhotonLibHypothesis::FillEstimate(const QCluster_t& trk, Flash_t &flash) const
  {
        

    flash.pe_v.resize(32,0.);
    for (auto &v : flash.pe_v) v = 0;

    for ( size_t ipmt = 0; ipmt < 32; ++ipmt) {
      
      std::vector<double> pt_last;
      for ( size_t ipt = 0; ipt < trk.size(); ++ipt) {
	
        auto const& pt = trk[ipt];

	double dist(0.0);
	if (pt_last.size() == 3 )
	  dist = std::sqrt(std::pow(pt_last.at(0)-pt.x,2.)+std::pow(pt_last.at(1)-pt.y,2.)+std::pow(pt_last.at(2)-pt.z,2.));
	pt_last.clear();
	pt_last.push_back(pt.x); pt_last.push_back(pt.y); pt_last.push_back(pt.z);

        double q = pt.q ;
	double q_corr = dist*2.2*29000;// *0.7; // cm * MeV/cm * photons/MeV * quenching(Efield)

	//		std::cout << "PhotonLibHypothesis: PMT : " << ipmt << " [x,y,z,t,q] -> [q_corr] : [" << pt.x << ", "
	//  << pt.y << ", " << pt.z << ", " << pt.t<< ", " << pt.q <<"] -> [" << q_corr << "]" << std::endl;
	//std::cout << "PhotonLibHypothesis: PMT : dist to last point is " <<  dist << std::endl;
	//if (::phot::PhotonVisibilityService::GetME().GetVisibility( pt.x, pt.y, pt.z, ipmt) == 0.)
	//  	  std::cout << "PhotonLibHypothesis: PMT : Look-up lib val is 0 for x,y,z,ipmt: " << pt.x << ", " << pt.y << ", " << pt.z << ", " << ipmt << std::endl;

        q *= ::phot::PhotonVisibilityService::GetME().GetVisibility( pt.x, pt.y, pt.z, ipmt)*0.23*0.0093; //Should be 0.23. //EC, 16-Dec-2015 
	//*0.0093;
        flash.pe_v[ipmt] += q;
	
	//std::cout << "PhotonLibHypothesis: PMT : " << ipmt << " [x,y,z,q] -> [q] : [" << pt.x << ", "
	//	  << pt.y << ", " << pt.z << ", " << pt.q <<"] -> [" << q << "]" << std::endl;

      }

    }


    return;
  }



}
#endif
