#ifndef ERTOOL_TRACK_CXX
#define ERTOOL_TRACK_CXX

#include "Track.h"

namespace ertool {

Track::Track() : RecoObjBase()
{
  _reco_type = kTrack;
  Reset();
}

void Track::Reset()
{
  _reco_type = kTrack;
  Trajectory::clear();
  RecoObjBase::Reset();

  _pid_score.clear();
  for (size_t i = 0; i < kTrackPartIDMax; ++i)
    _pid_score.push_back(-1);
  //    for(auto& v : _pid_score) v = -1;
  _pid = Track::kUnknown;
  _energy = kINVALID_DOUBLE;
  _time = kINVALID_DOUBLE;
}

void Track::PrintInfo() const {
  
  std::cout << "Printing track info:" << std::endl;
  std::cout << "    _energy = " << _energy << " MeV." << std::endl;
  std::cout << "    _time = " << _time << std::endl;
  std::cout << "    # of trajectory points = " << this->size() << std::endl;
  std::cout << "    Length = " << this->Length() << " cm." << std::endl;
  if (this->size()) {
    std::cout<<"    Start point = "<<this->at(0)<<std::endl;
    std::cout<<"    End point = "<<this->back()<<std::endl;
  }
  
}

}
#endif
