#ifndef SELECTIONTOOL_ERTOOL_EMPTYOUTPUT_CXX
#define SELECTIONTOOL_ERTOOL_EMPTYOUTPUT_CXX

#include "EmptyOutput.h"

namespace ertool {
  namespace io {

    EmptyOutput::EmptyOutput(const std::string& name)
      : _name(name)
    {}

    const std::string& EmptyOutput::Name() const
    { return _name; }
    
    void EmptyOutput::Prepare( ::ertool::EventData     *event_ptr,
			       ::ertool::ParticleGraph *graph_ptr,
			       bool mc)
    {}

    bool EmptyOutput::Open(const std::string& fname)
    {
      return true;
    }
      
    bool EmptyOutput::WriteEntry()
    {
      return true;
    }

    bool EmptyOutput::Close(const ::ertool::Provenance& prov)
    {
      return true;
    }
  }
}
#endif
