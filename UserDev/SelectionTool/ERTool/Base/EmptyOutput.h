/**
 * \file EmptyOutput.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class EmptyOutput
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_EMPTYOUTPUT_H
#define SELECTIONTOOL_ERTOOL_EMPTYOUTPUT_H

#include "OStreamBase.h"

namespace ertool {
  namespace io {
    /**
       \class EmptyOutput
       User defined class EmptyOutput ... these comments are used to generate
       doxygen documentation!
    */
    class EmptyOutput : public OStreamBase {
      
    public:
      
      /// Default constructor
      EmptyOutput(const std::string& name="noname");
      
      /// Default destructor
      virtual ~EmptyOutput(){}

      /// Name
      const std::string& Name() const;
      
      /// Function to prepare self before opening a file
      virtual void Prepare( ::ertool::EventData     *event_ptr,
			    ::ertool::ParticleGraph *graph_ptr,
			    bool mc);
      
      /// Function to "open" the input stream
      virtual bool Open(const std::string& fname);
      
      /// Function to "read" the input stream
      virtual bool WriteEntry();
      
      /// Function to "close" the output stream
      virtual bool Close(const ::ertool::Provenance& prov);
      
    private:
      std::string _name;
      
    };
  }
}
#endif
/** @} */ // end of doxygen group 

