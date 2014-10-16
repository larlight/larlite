#ifndef LARLITE_MCNU_CXX
#define LARLITE_MCNU_CXX

#include "mcnu.h"

namespace larlite {

  //***********************************************************
  mcnu::mcnu(mcpart &nu, mcpart &lep,
	     Int_t CCNC, Int_t mode, Int_t interactionType,
	     Int_t target, Int_t nucleon, Int_t quark,
	     Double_t w, Double_t x, Double_t y, Double_t qsqr)
    : data_base(data::kMCNeutrino),
      fNu(nu),
      fLepton(lep),
      fMode(mode),
      fInteractionType(interactionType),
      fCCNC(CCNC),
      fTarget(target),
      fHitNuc(nucleon),
      fHitQuark(quark),
      fW(w),
      fX(x),
      fY(y),
      fQSqr(qsqr)
  //***********************************************************
  {}

  //**********************
  void  mcnu::clear_data()
  //**********************
  {
    data_base::clear_data();
    fNu.clear_data();
    fLepton.clear_data();

    fMode            = data::kINVALID_INT;
    fInteractionType = data::kINVALID_INT;
    fCCNC            = data::kINVALID_INT;
    fTarget          = data::kINVALID_INT;
    fHitNuc          = data::kINVALID_INT;
    fHitQuark        = data::kINVALID_INT;
    fW               = data::kINVALID_DOUBLE;
    fX               = data::kINVALID_DOUBLE;
    fY               = data::kINVALID_DOUBLE;
    fQSqr            = data::kINVALID_DOUBLE;
  }

}
  
#endif
