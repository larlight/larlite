#ifndef ERTOOLBASE_FLASH_CXX
#define ERTOOLBASE_FLASH_CXX
#include "Flash.h"
#include "ERToolConstants.h"
namespace ertool {

  Flash::Flash(const ::geoalgo::Vector& pos)
    : ::geoalgo::Point_t ( pos )
    , _npe_v  (     )
    , _id     ( kINVALID_FLASH_ID )
  {}

  Flash::Flash()
    : ::geoalgo::Point_t ()
    , _id ( kINVALID_FLASH_ID )
  {}

  Flash::Flash(const ::geoalgo::Vector& pos,
	       const std::vector<double>& npe_v)
    : ::geoalgo::Point_t ( pos   )
    , _npe_v  ( npe_v )
    , _id     ( kINVALID_FLASH_ID )
  {}

  FlashID_t Flash::FlashID() const
  { return _id; }

  void Flash::Reset()
  {
    for(auto& v : (*this)) v = kINVALID_DOUBLE;
    _npe_v.clear();
  }

  void Flash::SetFlashID(const FlashID_t id)
  { _id = id; }
  
}
#endif
