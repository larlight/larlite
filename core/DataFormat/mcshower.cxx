#ifndef MCSHOWER_CXX
#define MCSHOWER_CXX

#include "mcshower.h"

namespace larlite {

  //-------------------------------------------------------------
  void mcshower::clear_data()
  //-------------------------------------------------------------
  {
    data_base::clear_data();
    fMotherPDGID   = data::kINVALID_INT;
    fMotherTrackID = data::kINVALID_UINT;
    fMotherProcess = "";
    fMotherVtx.clear();
    fMotherVtx.resize(4,0);
    fMotherMomentum.clear();
    fMotherMomentum.resize(4,0);
    fDaughterTrackID.clear();
    fDaughterMomentum.clear();
    fDaughterMomentum.resize(4,0);
    fDaughterVtx.clear();
    fDaughterVtx.resize(4,0);
    fPlaneCharge.clear();
  }

  void mcshower::SetMotherPoint(const std::vector<Double_t> &vtx)
  {
    if(vtx.size()!=4) {
      std::cerr<<"\033[93m"<<"Input must have 4 dimension (x,y,z,t)!"<<"\033[00m"<<std::endl;
      return;
    }
    fMotherVtx = vtx;
  }
  
  void mcshower::SetMotherMomentum(const std::vector<Double_t> &mom)
  {
    if(mom.size()!=4) {
      std::cerr<<"\033[93m"<<"Input must have 4 dimension (Px,Py,Pz,E)!"<<"\033[00m"<<std::endl;
      return;
    }
    fMotherMomentum = mom;
  }
  
  void mcshower::SetDaughterPosition(const std::vector<Double_t> &vtx)
  {
    if(vtx.size()!=4) {
      std::cerr<<"\033[93m"<<"Input must have 4 dimension (x,y,z,t)!"<<"\033[00m"<<std::endl;
      return;
    }
    fDaughterVtx = vtx;
  }

  void mcshower::SetDaughterMomentum(const std::vector<Double_t> &mom)
  {
    if(mom.size()!=4) {
      std::cerr<<"\033[93m"<<"Input must have 4 dimension (Px,Py,Pz,E)!"<<"\033[00m"<<std::endl;
      return;
    }
    fDaughterMomentum = mom;
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
