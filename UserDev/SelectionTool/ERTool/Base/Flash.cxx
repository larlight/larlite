#ifndef ERTOOLBASE_FLASH_CXX
#define ERTOOLBASE_FLASH_CXX
#include "Flash.h"
#include "ERToolConstants.h"
#include "ERException.h"
namespace ertool {

  Flash::Flash(const ::geoalgo::Vector& pos)
    : _npe_v  ()
    , _t      ( kINVALID_DOUBLE   )
    , _id     ( kINVALID_FLASH_ID )
  {
    if(pos.size()!=3) throw ERException("Flash must be 3D point!");
    _x = pos[0];
    _y = pos[1];
    _z = pos[2];
  }

  Flash::Flash()
    : _npe_v  ()
    , _x      ( kINVALID_DOUBLE  )
    , _y      ( kINVALID_DOUBLE   )
    , _z      ( kINVALID_DOUBLE   )
    , _t      ( kINVALID_DOUBLE   )
    , _id     ( kINVALID_FLASH_ID )
  {}

  Flash::Flash(const ::geoalgo::Vector& pos,
	       const std::vector<double>& npe_v)
    : _npe_v  ( npe_v )
    , _t      ( kINVALID_DOUBLE   )
    , _id     ( kINVALID_FLASH_ID )
  {
    if(pos.size()!=3) throw ERException("Flash must be 3D point!");
    _x = pos[0];
    _y = pos[1];
    _z = pos[2];
  }

  FlashID_t Flash::FlashID() const
  { return _id; }

  double Flash::TotalPE() const
  { double sum=0;
    for(auto const& v : _npe_v) sum += v;
    return sum;
  }

  void Flash::Reset()
  {
    _x = _y = _z = _t = kINVALID_DOUBLE;
    _npe_v.clear();
  }

  void Flash::SetFlashID(const FlashID_t id)
  { _id = id; }
  
}
#endif
