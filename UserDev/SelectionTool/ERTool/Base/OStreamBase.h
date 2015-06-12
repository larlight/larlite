/**
 * \file OStreamBase.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class OStreamBase
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_OSTREAMBASE_H
#define SELECTIONTOOL_ERTOOL_OSTREAMBASE_H

#include "ParticleGraph.h"
#include "EventData.h"
#include "Provenance.h"

namespace ertool {
  /**
     \class OStreamBase
     User defined class OStreamBase ... these comments are used to generate
     doxygen documentation!
  */
  class OStreamBase{
    
  public:
    
    /// Default constructor
    OStreamBase(){}
    
    /// Default destructor
    virtual ~OStreamBase(){}

    /// Function to "open" the input stream
    virtual bool Open() = 0;

    /// Function to "read" the input stream
    virtual bool Write() = 0;
    
    /// Function to "close" the output stream
    virtual bool Close(Provenance* prov) = 0;

    /// Set an output file
    virtual void SetFile( const std::string& fname ) = 0;

  protected:

    /// Function to "set data pointer"
    virtual void SetDataPointer(EventData* data, ParticleGraph* graph) = 0;

    /// Function to "set mc data pointer"
    virtual void SetMCPointer(EventData* data, ParticleGraph* graph) = 0;
    
  };
}
#endif
/** @} */ // end of doxygen group 

