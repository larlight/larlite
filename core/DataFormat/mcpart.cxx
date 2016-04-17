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

  void mcpart::AddDaughter     (const int trackID)   { fdaughters.insert(trackID);  }
  void mcpart::SetPolarization (TVector3 const& p)   { fpolarization = p;           }
  void mcpart::SetRescatter    (int code)            { frescatter    = code;        }
  void mcpart::SetWeight       (double wt)           { fWeight       = wt;          }
  void mcpart::SetEndProcess   (std::string s)       { fendprocess   = s;           }
  void mcpart::SetTrajectory   (const mctrajectory steps) { ftrajectory   = steps;  }
  void mcpart::AddTrajectory   (const mcstep step)   { ftrajectory.push_back(step); }
  void mcpart::AddTrajectory   (const TLorentzVector& position,
			const TLorentzVector& momentum)
  { ftrajectory.push_back(position,momentum); }
  
  void mcpart::AddFiducialTrack(size_t start, size_t end)
  { ftrackFiducial.push_back(std::pair<size_t,size_t>(start,end)); }
  
  void mcpart::AddFiducialTrack(const std::pair<size_t,size_t>& step)
  { ftrackFiducial.push_back(step); }
  
  int                  mcpart::StatusCode()   const { return fstatus;       }
  int                  mcpart::TrackId()      const { return ftrackId;      }
  int                  mcpart::PdgCode()      const { return fpdgCode;      }
  int                  mcpart::Mother()       const { return fmother;       }
  const std::string    mcpart::Process()      const { return fprocess;      }
  const std::string    mcpart::EndProcess()   const { return fendprocess;   }
  
  const mctrajectory&  mcpart::Trajectory()   const { return ftrajectory;   }
  double               mcpart::Mass()         const { return fmass;         }
  const TVector3&      mcpart::Polarization() const { return fpolarization; }
  const std::set<int>& mcpart::Daughters()    const { return fdaughters;    } 
  double               mcpart::Weight()       const { return fWeight;       }
  TLorentzVector       mcpart::GetGvtx()      const { return fGvtx;         }
  int                  mcpart::Rescatter()    const { return frescatter;    }
  const std::vector<std::pair<size_t,size_t> >& mcpart::FiducialTrack() const 
  {return ftrackFiducial;}

  const TLorentzVector& mcpart::Position(size_t i) const
  {
    if(i>=ftrajectory.size()) throw DataFormatException("Invalid index requested!");
    return ftrajectory[i].Position();
  }

  const TLorentzVector& mcpart::Momentum(size_t i) const
  {
    if(i>=ftrajectory.size()) throw DataFormatException("Invalid index requested!");
    return ftrajectory[i].Momentum();
  }
}

#endif
