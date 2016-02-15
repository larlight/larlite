#ifndef LARLITE_MAKEFAKESHOWERS_CXX
#define LARLITE_MAKEFAKESHOWERS_CXX

#include "MakeFakeShowers.h"
#include "DataFormat/mcshower.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include <math.h>

namespace larlite {

  bool MakeFakeShowers::initialize() {

    return true;
  }
  
  bool MakeFakeShowers::analyze(storage_manager* storage) {
  
    auto ev_shr = storage->get_data<event_mcshower>("fakeshower");

    storage->set_id(storage->run_id(),storage->subrun_id(),storage->event_id());
    
    for (size_t i=0; i < 10; i++){
      mcshower shr;
      double sqrt3 = 1./sqrt(3.);
      shr.PdgCode(11);
      shr.TrackID(i);
      shr.Process("fake");
      mcstep start(TLorentzVector(0,0,0,0),TLorentzVector(sqrt3,sqrt3,sqrt3,200));
      shr.Start(start);
      shr.StartDir(TVector3(sqrt3,sqrt3,sqrt3));
      shr.DetProfile(start);
      shr.dQdx( 5e4 );
      shr.dEdx( 2.0 );

      ev_shr->push_back(shr);
    }
  
    return true;
  }

  bool MakeFakeShowers::finalize() {

    return true;
  }

}
#endif
