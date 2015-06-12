#ifndef SELECTIONTOOL_ERTOOL_ROOTINPUT_CXX
#define SELECTIONTOOL_ERTOOL_ROOTINPUT_CXX

#include "ROOTInput.h"

namespace ertool {

  /// Function to "open" the input stream
  bool Open()
  { return true; }
  
  /// Function to "read" the input stream
  bool Read(size_t entry)
  { return true; }

  /// Function to "close" the output stream
  bool Close()
  { return true; }
  
  /// Add an input file
  void AddFile( const std::string& fname )
  { }
  
  /// Function to "set data pointer"
  void SetDataPointer(EventData* data, ParticleGraph* graph)
  { }
  
  /// Function to "set mc data pointer"
  void SetMCPointer(EventData* data, ParticleGraph* graph)
  { }
  
}
#endif
