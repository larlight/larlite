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

    fdEdx.clear();
    fdQdx.clear();

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
  
  int                mctrack::AncestorPdgCode             () const { return fAncestorPDGCode; }
  unsigned int       mctrack::AncestorTrackID             () const { return fAncestorTrackID; }
  const std::string& mctrack::AncestorProcess             () const { return fMotherProcess;   }
  const mcstep&      mctrack::AncestorStart               () const { return fAncestorStart;   }
  const mcstep&      mctrack::AncestorEnd                 () const { return fAncestorEnd;     }
  const std::vector<double>& mctrack::dEdx                () const { return fdEdx;            }
  const std::vector<std::vector <double> >& mctrack::dQdx () const { return fdQdx;            }




  /*
    double   mctrack::dX(const SegmentID_t id) const {
    if (id + 2 > this->size()) {
      std::cerr<<"Invalid segment ID "<<id<<std::endl;
      throw DataFormatException();
    }


    auto const& previousStep = (*this)[id];
    auto const& nextStep     = (*this)[id+1];
    double dX =
      (previousStep.X() - nextStep.X())*(previousStep.X() - nextStep.X()) +
      (previousStep.Y() - nextStep.Y())*(previousStep.Y() - nextStep.Y()) +
      (previousStep.Z() - nextStep.Z())*(previousStep.Z() - nextStep.Z())  ;

    dX = TMath::Sqrt(dX);
    
    return dX;
  }


  double   mctrack::dEdx(const SegmentID_t id) const {
    if (id + 2 > this->size()) {
      std::cerr<<"Invalid segment ID "<<id<<std::endl;
      throw DataFormatException();
    }

    double dEdX;

    auto const& previousStep = (*this)[id];
    auto const& nextStep     = (*this)[id+1];
    double stepDistance = dX(id);
         
    stepDistance = TMath::Sqrt(stepDistance);
    double dE = TMath::Abs(nextStep.E() - previousStep.E());
    dEdX  = dE/stepDistance;
    
    return dEdX;

  }


  const std::vector<double>  mctrack::dEdx() const {
    std::vector<double> fdEdxComputed;
    if (this->size()<2) return fdEdxComputed;
    
    fdEdxComputed.reserve(this->size()-1);
    for (SegmentID_t i = 0; i + 1 < this->size(); ++i)
      {
	fdEdxComputed.push_back(dEdx(i));
      }
    return fdEdxComputed;
  }
  
  const std::vector<TLorentzVector>  mctrack::SegmentCenter() const{
    std::vector<TLorentzVector> fsegmentCenter;
    if (this->size()<2) return fsegmentCenter;

    fsegmentCenter.reserve(size()-1);
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
  */


}
#endif
  
