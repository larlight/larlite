#ifndef ERTOOL_UNITBASE_CXX
#define ERTOOL_UNITBASE_CXX

#include "UnitBase.h"
#include "ERException.h"
namespace ertool {

  UnitBase::UnitBase(const std::string& name)
    : _training_mode(false)
    , _name(name)
    , _out_conf(name)
  {
    if(_name.empty())
      throw ERException("Cannot instantiate ER Ana/Algo with no name!");
  }
  
  const std::string& UnitBase::Name() const
  {
    return _name;
  }

  ::fcllite::PSet& UnitBase::OutputPSet()
  { return _out_conf; }

}
#endif
