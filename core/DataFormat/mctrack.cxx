#ifndef LARLITE_MCTRACK_CXX
#define LARLITE_MCTRACK_CXX

#include "mctrack.h"

namespace larlite {


  void mctrack::clear_data()
  {
    std::vector<mcstep>::clear();
    data_base::clear_data();

    fOrigin  = simb::kUnknown;
    fProcess = "";
    fPDGCode         = data::kINVALID_INT;
    fTrackID         = data::kINVALID_UINT;

    fMotherPDGCode   = data::kINVALID_INT;
    fMotherTrackID   = data::kINVALID_UINT;
    fMotherProcess   = "";

    fAncestorPDGCode   = data::kINVALID_INT;
    fAncestorTrackID   = data::kINVALID_UINT;
    fAncestorProcess   = "";

    TLorentzVector invalid(data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE,
			   data::kINVALID_DOUBLE);

    mcstep invalid_step(invalid,invalid);

    fStart = invalid_step;
    fEnd   = invalid_step;

    fMotherStart = invalid_step;
    fMotherEnd   = invalid_step;
    
    fAncestorStart = invalid_step;
    fAncestorEnd   = invalid_step;
  
  }

  simb::Origin_t      mctrack::Origin     () const { return fOrigin;          }
  int                 mctrack::PdgCode    () const { return fPDGCode;         }
  unsigned int        mctrack::TrackID    () const { return fTrackID;         }
  const std::string&  mctrack::Process    () const { return fProcess;         }
  const mcstep&       mctrack::Start      () const { return fStart;           }
  const mcstep&       mctrack::End        () const { return fEnd;             }
  
  int                mctrack::MotherPdgCode () const { return fMotherPDGCode;   }
  unsigned int       mctrack::MotherTrackID () const { return fMotherTrackID;   }
  const std::string& mctrack::MotherProcess () const { return fMotherProcess;   }
  const mcstep&      mctrack::MotherStart   () const { return fMotherStart;     }
  const mcstep&      mctrack::MotherEnd     () const { return fMotherEnd;       }
  
  int                mctrack::AncestorPdgCode   () const { return fAncestorPDGCode; }
  unsigned int       mctrack::AncestorTrackID   () const { return fAncestorTrackID; }
  const std::string& mctrack::AncestorProcess   () const { return fMotherProcess;   }
  const mcstep&      mctrack::AncestorStart     () const { return fAncestorStart;   }
  const mcstep&      mctrack::AncestorEnd       () const { return fAncestorEnd;     }


  const std::vector<double>  mctrack::dEdx() const {
    std::vector<double> fdEdxComputed;
    if (this->size()<2) return fdEdxComputed;

    fdEdxComputed.reserve(this->size()-1);
    for (unsigned int i = 1; i < this->size(); ++i)
      {
	auto const& previousStep = (*this)[i-1];
	auto const& nextStep     = (*this)[i];
	double stepDistance =
	  (previousStep.X() - nextStep.X())*(previousStep.X() - nextStep.X()) +
	  (previousStep.Y() - nextStep.Y())*(previousStep.Y() - nextStep.Y()) +
	  (previousStep.Z() - nextStep.Z())*(previousStep.Z() - nextStep.Z())  ;
	
	stepDistance = TMath::Sqrt(stepDistance);
	double dE = TMath::Abs(nextStep.E() - previousStep.E());	    
	double currentdEdx = dE/stepDistance;
	fdEdxComputed.push_back(currentdEdx);

      }
    return fdEdxComputed;
  }

  const std::vector<TLorentzVector>  mctrack::SegmentCenter() const{
    std::vector<TLorentzVector> fsegmentCenter;
    if (this->size()<2) return fsegmentCenter;

    fsegmentCenter.reserve(this->size()-1);
    for (unsigned int i = 1; i < this->size(); ++i) {
      auto const& previousStep = (*this)[i-1];
      auto const& nextStep     = (*this)[i];

      double xSegCenter = (previousStep.X() + nextStep.X())*0.5 ;
      double ySegCenter = (previousStep.Y() + nextStep.Y())*0.5 ;
      double zSegCenter = (previousStep.Z() + nextStep.Z())*0.5 ;
      double tSegCenter = (previousStep.T() + nextStep.T())*0.5 ;

      TLorentzVector currentSegCenter(xSegCenter,ySegCenter,zSegCenter,tSegCenter);
      fsegmentCenter.emplace_back(currentSegCenter);
    }
    return fsegmentCenter;
  }



}
#endif
  
