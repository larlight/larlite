/**
 * \file IStreamBase.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class IStreamBase
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_ISTREAMBASE_H
#define SELECTIONTOOL_ERTOOL_ISTREAMBASE_H

#include "ParticleGraph.h"
#include "EventData.h"

namespace ertool {
  /**
     \class IStreamBase
     User defined class IStreamBase ... these comments are used to generate
     doxygen documentation!
  */
  class IStreamBase{
    
  public:
    
    /// Default constructor
    IStreamBase(){}
    
    /// Default destructor
    virtual ~IStreamBase(){}

    /// Function to "open" the input stream
    virtual bool Open() = 0;

    /// Function to "read" the input stream
    virtual bool Read(size_t entry) = 0;

    /// Function to "close" the output stream
    virtual bool Close() = 0;

    /// Add an input file
    virtual void AddFile( const std::string& fname ) = 0;

  protected:

    /// Function to "set data pointer"
    virtual void SetDataPointer(EventData* data, ParticleGraph* graph) = 0;

    /// Function to "set mc data pointer"
    virtual void SetMCPointer(EventData* data, ParticleGraph* graph) = 0;

  };
}

#endif
/** @} */ // end of doxygen group 

