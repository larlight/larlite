#ifndef LARLITE_MCSHOWER_CXX
#define LARLITE_MCSHOWER_CXX

#include "mcshower.h"

namespace larlite {

  //-------------------------------------------------------------
  void mcshower::clear_data()
  //-------------------------------------------------------------
  {
    data_base::clear_data();
    TLorentzVector invalid(data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE);
    mcstep invalid_step(invalid,invalid);

    fOrigin    = simb::kUnknown;

    fPDGCode   = data::kINVALID_INT;
    fG4TrackID = data::kINVALID_UINT;
    fProcess   = "";
    fG4Start   = invalid_step;
    fG4End     = invalid_step;

    fMotherPDGCode   = data::kINVALID_INT;
    fMotherG4TrackID = data::kINVALID_UINT;
    fMotherProcess   = "";
    fMotherG4Start   = invalid_step;
    fMotherG4End     = invalid_step;

    fAncestorPDGCode   = data::kINVALID_INT;
    fAncestorG4TrackID = data::kINVALID_UINT;
    fAncestorProcess   = "";
    fAncestorG4Start   = invalid_step;
    fAncestorG4End     = invalid_step;

    fDetProfile = invalid_step;
    
    fDaughterTrackID.clear();
    fPlaneCharge.clear();
  }

  //----------------------------------------------------
  Double_t mcshower::Charge(const size_t plane) const
  //----------------------------------------------------
  {
    if(plane > fPlaneCharge.size()) {

      std::cerr<<"\033[93m"<<"No charge stored for plane: "<<plane<<"\033[00m"<<std::endl;
      return -1;

    }
      
    return fPlaneCharge[plane];
  }

}

#endif
