//
// type-def file for ROOT
//

#ifndef RECOTOOL_SHOWERRECO3D_TYPEDEF_H
#define RECOTOOL_SHOWERRECO3D_TYPEDEF_H

// Header files included here
#include "ShowerReco3D.h"
#include "ComputePi0Mass.h"
#include "ShowerQuality.h"
#include "HitCalibration.h"
//ADD_NEW_HEADER ... do not change this comment line
#include "ShowerRecoException.h"
#include "ShowerCalo.h"
#include "ShowerRecoAlgBase.h"
#include "ShowerRecoAlg.h"
#include "ShowerRecoTest.h"
#include "Pi0ShowerRecoAlg.h"

//ADD_NEW_ALG_HEADER
namespace showerreco {
  class ShowerRecoException;
  class ShowerRecoAlgBase;
  class ShowerCalo;
  class ShowerRecoAlg;
  class ShowerRecoTest;
  class Pi0ShowerRecoAlg;
  //ADD_NEW_ALG_CLASS
}

namespace larlite {
  class ShowerReco3D;
  class ComputePi0Mass;
  class ShowerQuality;
  class HitCalibration;
//ADD_ANA_CLASS ... do not change this comment line
}

//ADD_EMPTY_CLASS ... do not change this comment line
#endif
