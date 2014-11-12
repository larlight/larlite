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
    fTrackID = data::kINVALID_UINT;
    fProcess   = "";
    fStart   = invalid_step;
    fEnd     = invalid_step;

    fMotherPDGCode   = data::kINVALID_INT;
    fMotherTrackID = data::kINVALID_UINT;
    fMotherProcess   = "";
    fMotherStart   = invalid_step;
    fMotherEnd     = invalid_step;

    fAncestorPDGCode   = data::kINVALID_INT;
    fAncestorTrackID = data::kINVALID_UINT;
    fAncestorProcess   = "";
    fAncestorStart   = invalid_step;
    fAncestorEnd     = invalid_step;

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
