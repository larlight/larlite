#ifndef LARLITE_MCPART_CXX
#define LARLITE_MCPART_CXX

#include "mcpart.h"

namespace larlite {

  //********************************************
  mcpart::mcpart(const Int_t       trackId,
		 const Int_t       pdgid,
		 const std::string process,
		 const Int_t       mother,
		 const Double_t    mass,
		 const Int_t       status)
    : data_base(data::kMCParticle), 
      ftrajectory(),
      fpolarization(),
      fdaughters(),
      fGvtx(),
      ftrackFiducial()
  //********************************************
  {
    clear_data();
    ftrackId = trackId;
    fpdgCode = pdgid;
    fprocess = process;
    fmother  = mother;
    fmass    = mass;
    fstatus  = status;
  }

  //***********************
  void mcpart::clear_data() 
  //***********************
  {
    data_base::clear_data();
    fstatus  = data::kINVALID_INT;
    ftrackId = data::kINVALID_INT;
    fpdgCode = data::kINVALID_INT;
    fmother  = data::kINVALID_INT;
    fprocess = "";
    ftrajectory.clear_data();
    fmass    = data::kINVALID_DOUBLE;
    fpolarization.Clear();
    fdaughters.clear();
    fWeight  = data::kINVALID_DOUBLE;
    fGvtx.Clear();
    ftrackFiducial.clear();
  }

}

#endif
