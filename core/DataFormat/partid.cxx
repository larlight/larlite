#ifndef LARLITE_PARTID_CXX
#define LARLITE_PARTID_CXX

#include "partid.h"
#include <iomanip>

namespace larlite {

  //#################################################
  partid::partid()
  //#################################################
    : data_base(data::kParticleID)
    , fPdg(0)
    , fNdf(0)
    , fMinChi2(0.)
    , fDeltaChi2(0.)
    , fChi2Proton(0.)
    , fChi2Kaon(0.)
    , fChi2Pion(0.)
    , fChi2Muon(0.)
    , fMissingE(0.)
    , fMissingEavg(0.)
    , fPIDA(0.)
    , fPlaneID()
  {
    clear_data();
  }

  partid::partid(int Pdg,
		 int Ndf,
		 double MinChi2,
		 double DeltaChi2,
		 double Chi2Proton,
		 double Chi2Kaon,
		 double Chi2Pion,
		 double Chi2Muon,
		 double MissingE,
		 double MissingEavg,
		 double PIDA,
		 geo::PlaneID planeid)
    : fPdg(Pdg)
    , fNdf(Ndf)
    , fMinChi2(MinChi2)
    , fDeltaChi2(DeltaChi2)
    , fChi2Proton(Chi2Proton)
    , fChi2Kaon(Chi2Kaon)
    , fChi2Pion(Chi2Pion)
    , fChi2Muon(Chi2Muon)
    , fMissingE(MissingE)
    , fMissingEavg(MissingEavg)
    , fPIDA(PIDA)
    , fPlaneID(planeid)
  {}

  std::ostream& operator<< (std::ostream & o, partid const& a)
  {
    o << "Determined particle PDG code: " << a.fPdg
      << "\n NDF: "                       << a.fNdf
      << "\n Minimal chi2: "              << a.fMinChi2
      << "\n Delta chi2: "                << a.fDeltaChi2
      << "\n chi2 with proton template: " << a.fChi2Proton
      << "\n chi2 with kaon template: "   << a.fChi2Kaon
      << "\n chi2 with pion template: "   << a.fChi2Pion
      << "\n chi2 with muon template: "   << a.fChi2Muon
      << "\n PIDA:                    "   << a.fPIDA
      <<std::endl;


    return o;
  }


  //##########################################################################
  void partid::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
  }

}
#endif
  
