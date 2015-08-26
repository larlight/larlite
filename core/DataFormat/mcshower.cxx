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
    
    TVector3 invalid_vec(data::kINVALID_DOUBLE,
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

    fStartDir = invalid_vec;

    fdEdx = data::kINVALID_DOUBLE;
    fdQdx.clear();
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

  //----------------------------------------------------
  Double_t mcshower::dQdx(const size_t plane) const
  //----------------------------------------------------
  {
    if(plane > fdQdx.size()) {

      std::cerr<<"\033[93m"<<"No charge stored for plane: "<<plane<<"\033[00m"<<std::endl;
      return -1;

    }
      
    return fdQdx[plane];
  }

  
  simb::Origin_t     mcshower::Origin() const { return fOrigin;    }
  
  int                mcshower::PdgCode () const { return fPDGCode;   }
  unsigned int       mcshower::TrackID () const { return fTrackID; } 
  const std::string& mcshower::Process () const { return fProcess;   }
  const mcstep&      mcshower::Start   () const { return fStart;   }
  const mcstep&      mcshower::End     () const { return fEnd;     }

  int                mcshower::MotherPdgCode () const { return fMotherPDGCode;   }
  unsigned int       mcshower::MotherTrackID () const { return fMotherTrackID; }
  const std::string& mcshower::MotherProcess () const { return fMotherProcess;   }
  const mcstep&      mcshower::MotherStart   () const { return fMotherStart;   }
  const mcstep&      mcshower::MotherEnd     () const { return fMotherEnd;     }

  int                mcshower::AncestorPdgCode () const { return fAncestorPDGCode;   }
  unsigned int       mcshower::AncestorTrackID () const { return fAncestorTrackID; }
  const std::string& mcshower::AncestorProcess () const { return fAncestorProcess;   }
  const mcstep&      mcshower::AncestorStart   () const { return fAncestorStart;   }
  const mcstep&      mcshower::AncestorEnd     () const { return fAncestorEnd;     }

  const mcstep& mcshower::DetProfile () const { return fDetProfile; }
  
  const std::vector<unsigned int>&  mcshower::DaughterTrackID() const { return fDaughterTrackID; }
  
  const std::vector<double>& mcshower::Charge() const { return fPlaneCharge; }

  const TVector3& mcshower::StartDir() const { return fStartDir; }
  const double& mcshower::dEdx() const { return fdEdx; }

  const std::vector<double>& mcshower::dQdx() const { return fdQdx; }

}

#endif
