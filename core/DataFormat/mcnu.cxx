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

  mcnu::mcnu(const mcnu& origin)
    : data_base(origin),
      fNu(origin.fNu),
      fLepton(origin.fLepton),
      fMode(origin.fMode),
      fInteractionType(origin.fInteractionType),
      fCCNC(origin.fCCNC),
      fTarget(origin.fTarget),
      fHitNuc(origin.fHitNuc),
      fHitQuark(origin.fHitQuark),
      fW(origin.fW),
      fX(origin.fX),
      fY(origin.fY),
      fQSqr(origin.fQSqr)
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
  
  const  mcpart&  mcnu::Nu()     const { return fNu;              }
  const  mcpart&  mcnu::Lepton() const { return fLepton;          }
  int    mcnu::CCNC()            const { return fCCNC;            }
  int    mcnu::Mode()            const { return fMode;            }
  int    mcnu::InteractionType() const { return fInteractionType; }
  int    mcnu::Target()          const { return fTarget;          }
  int    mcnu::HitNuc()          const { return fHitNuc;          }
  int    mcnu::HitQuark()        const { return fHitQuark;        }
  double mcnu::W()               const { return fW;               }
  double mcnu::X()               const { return fX;               }
  double mcnu::Y()               const { return fY;               }
  double mcnu::QSqr()            const { return fQSqr;            }
  
}
  
#endif
