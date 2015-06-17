#ifndef ERTOOL_RECOOBJBASE_CXX
#define ERTOOL_RECOOBJBASE_CXX

#include "RecoObjBase.h"

namespace ertool {

  RecoObjBase::RecoObjBase()
    : _reco_id(kINVALID_RECO_ID)
    , _reco_type(kInvisible)
  {}

  RecoID_t RecoObjBase::RecoID() const
  { return _reco_id; }

  RecoType_t RecoObjBase::RecoType() const
  {
    return _reco_type;
  }
  
  void RecoObjBase::SetRecoInfo(const RecoID_t id, const RecoType_t type)
  {
    _reco_id = id;
    _reco_type = type;
  }

}

#endif
