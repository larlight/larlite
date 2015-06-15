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
  namespace io {
    /**
       \class OStreamBase
       User defined class OStreamBase ... these comments are used to generate
       doxygen documentation!
    */
    class OStreamBase{
      
    public:
      
      /// Default constructor
      OStreamBase(const std::string& name="noname");
      
      /// Default destructor
      virtual ~OStreamBase(){}

      /// Name
      const std::string& Name() const;
      
      /// Function to prepare self before opening a file
      virtual void Prepare( ::ertool::EventData     *event_ptr,
			    ::ertool::ParticleGraph *graph_ptr,
			    bool mc) = 0;
      
      /// Function to "open" the input stream
      virtual bool Open(const std::string& fname) = 0;
      
      /// Function to "read" the input stream
      virtual bool WriteEntry() = 0;
      
      /// Function to "close" the output stream
      virtual bool Close(const ::ertool::Provenance& prov) = 0;
      
    private:
      std::string _name;
      
    };
  }
}
#endif
/** @} */ // end of doxygen group 

