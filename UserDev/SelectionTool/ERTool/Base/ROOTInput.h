/**
 * \file ROOTInput.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class ROOTInput
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_ROOTINPUT_H
#define SELECTIONTOOL_ERTOOL_ROOTINPUT_H

#include "IStreamBase.h"

namespace ertool {
  /**
     \class ROOTInput
     User defined class ROOTInput ... these comments are used to generate
     doxygen documentation!
  */
  class ROOTInput : public IStreamBase {
    
  public:
    
    /// Default constructor
    ROOTInput(){}
    
    /// Default destructor
    ~ROOTInput(){}

    /// Function to "open" the input stream
    bool Open();

    /// Function to "read" the input stream
    bool Read(size_t entry);

    /// Function to "close" the output stream
    bool Close();

    /// Add an input file
    void AddFile( const std::string& fname );

  protected:

    /// Function to "set data pointer"
    void SetDataPointer(EventData* data, ParticleGraph* graph);

    /// Function to "set mc data pointer"
    void SetMCPointer(EventData* data, ParticleGraph* graph);

  };
}

#endif
/** @} */ // end of doxygen group 

