#ifndef PARTICLECONVERTER_CXX
#define PARTICLECONVERTER_CXX

#include "ParticleConverter.h"

namespace larlite {

  ::ertool::RecoObjBase ParticleConverter::Convert(const mcpart part) const
  {

    if ( (part.PdgCode() == 11) || (part.PdgCode() == -11) ){
      ::geoalgo::Vector_t mom(part.Trajectory().at(0).Momentum());
      mom.Normalize();
      ::ertool::Shower s(part.Trajectory().at(0).Position(), mom,
			 _shrProfiler.Length(part.Trajectory().at(0).E()),
			 _shrProfiler.ShowerRadius() );
      return s;
    }// if shower

    if ( (part.PdgCode() == 13) || (part.PdgCode() == -13) ){
      ::ertool::Track t;
      for (auto& step : part.Trajectory())
	t.push_back(step.Position());
      return t;
    }// if shower
  
  // Else return an empty RecoObjBase
  ::ertool::RecoObjBase x;
  return x;
  }

}

#endif
