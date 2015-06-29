#ifndef ERTOOL_EMPTYINPUT_CXX
#define ERTOOL_EMPTYINPUT_CXX

#include "ERException.h"
#include "EmptyInput.h"

namespace ertool {

  namespace io {

    const std::string& EmptyInput::Name() const
    { return _name; }
    
    
    size_t EmptyInput::NumEntries() const
    { return 0; }
      
    void EmptyInput::Prepare( ::ertool::EventData     *event_ptr,
			      ::ertool::ParticleGraph *graph_ptr,
			      bool mc)
    { return; }
    
    void EmptyInput::AddFile( const std::string& fname )
    {
      throw ERException("EmptyInput does not take an input file ");
    }
      
    bool EmptyInput::Open( ::ertool::Provenance& in_prov)
    {
      return true;
    }
      
    bool EmptyInput::ReadParticleGraph(const size_t entry)
    {
      return true;
    }

    bool EmptyInput::ReadEventData(const size_t entry)
    {
      return true;
    }

    bool EmptyInput::Close()
    {
      return true;
    }
  }
}

#endif
