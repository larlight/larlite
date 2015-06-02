#ifndef ERTOOL_PROVENANCE_CXX
#define ERTOOL_PROVENANCE_CXX

#include "Provenance.h"
#include "ERException.h"
namespace ertool {

  Provenance::Provenance(const std::string& name)
    : _proc_id   (    0 )
    , _proc_name ( name )
  {}

  void Provenance::SetInput(const Provenance& input)
  {
    _input_info  = input.InputInfo();
    _input_name  = input.ProcessName();
    _proc_id     = input.ProcessID() + 1;
  }

  void Provenance::AddInfo(const std::string& key,
			   const std::string& value)
  {
    if( _info.find(key) != _info.end() )
      throw ERException("Cannot add a duplicate information key...");
    _info[key]=value;
  }


}
#endif
