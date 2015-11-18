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
    if (this->size()>2)
      {
	mcstep previousStep = this->fStart;
	for (unsigned int i = 1; i < this->size(); ++i)
	  {
	    double stepDistance =
	      (previousStep.X() - this->at(i).X())*(previousStep.X() - this->at(i).X()) +
	      (previousStep.Y() - this->at(i).Y())*(previousStep.Y() - this->at(i).Y()) +
	      (previousStep.Z() - this->at(i).Z())*(previousStep.Z() - this->at(i).Z())  ;
	    
	    stepDistance = TMath::Sqrt(stepDistance);
	    double dE = TMath::Abs(this->at(i).E() - previousStep.E());	    
	    double currentdEdx = dE/stepDistance;
	    fdEdxComputed.push_back(currentdEdx);
	    previousStep = this->at(i);
	  }
      } else 
      {
	std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
		  <<"dEdX impossible to compute, the track is not long enough\n";
	fdEdxComputed.clear();
       
      }
    return fdEdxComputed; 
  }


  const std::vector<TLorentzVector>  mctrack::segmentCenter() const{
    std::vector<TLorentzVector> fsegmentCenter;
    if (this->size()>2)
      {
        mcstep previousStep = this->fStart;
        for (unsigned int i = 1; i < this->size(); ++i)
          {
            double xSegCenter = (previousStep.X() - this->at(i).X())*0.5 ;
	    double ySegCenter = (previousStep.Y() - this->at(i).Y())*0.5 ;
	    double zSegCenter = (previousStep.Z() - this->at(i).Z())*0.5 ;
	    double tSegCenter = (previousStep.T() - this->at(i).T())*0.5 ;

            TLorentzVector currentSegCenter(xSegCenter,ySegCenter,zSegCenter,tSegCenter);
            fsegmentCenter.push_back(currentSegCenter);
            previousStep = this->at(i);
          }
      } else
      {
	std::cerr << "\033[93m<<" << __FUNCTION__ << ">>\033[00m"
		  <<"segment center impossible to compute, the track has < 2 mcsteps\n";
	fsegmentCenter.clear();
      }
    return fsegmentCenter;
  }



}
#endif
  
