#ifndef ERTOOL_PROVENANCE_CXX
#define ERTOOL_PROVENANCE_CXX

#include <cassert>
#include "Provenance.h"
#include "ERException.h"

namespace ertool {

  Provenance::Provenance(const std::string& name)
    : _proc_name_v(1,name)
    , _proc_info_v(1)
  {
    assert(!name.empty());
  }

  Provenance::Provenance(const Provenance& input, const std::string& name)
    : _proc_name_v(input._proc_name_v)
    , _proc_info_v(input._proc_info_v)
  {
    assert(!name.empty());
    _proc_info_v.push_back(ProcInfo_t());
    _proc_name_v.push_back(name);
  }

  void Provenance::AddInfo(const std::string& key,
			   const std::string& value)
  {
    auto& info = _proc_info_v.back();
    if( info.find(key) != info.end() )
      throw ERException("Cannot add a duplicate information key...");
    info[key]=value;
  }

  const ertool::ProcInfo_t& Provenance::Info(const size_t proc_id) const
  {
    if(proc_id == kINVALID_SIZE) return _proc_info_v.back();
    if(proc_id >= _proc_info_v.size())
      throw ERException("Invalid process ID provided: no provenance info available!");
    return _proc_info_v[proc_id];
  }
  
  const std::string& Provenance::Name(const size_t proc_id) const
  {
    if(proc_id == kINVALID_SIZE) return _proc_name_v.back();
    if(proc_id >= _proc_name_v.size())
      throw ERException("Invalid process ID provided: no provenance info available!");
    return _proc_name_v[proc_id];    
  }

}
#endif
