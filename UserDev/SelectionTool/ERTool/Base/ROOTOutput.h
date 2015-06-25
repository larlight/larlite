/**
 * \file ROOTOutput.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class ROOTOutput
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_ROOTOUTPUT_H
#define SELECTIONTOOL_ERTOOL_ROOTOUTPUT_H

#include "OStreamBase.h"
#include <TFile.h>
#include <TTree.h>

namespace ertool {
  namespace io {
    /**
       \class ROOTOutput
       User defined class ROOTOutput ... these comments are used to generate
       doxygen documentation!
    */
    class ROOTOutput : public OStreamBase {
      
    public:
      
      /// Default constructor
      ROOTOutput(const std::string& name="ROOTOutput");
      
      /// Default destructor
      ~ROOTOutput(){}

      /// Function to prepare self before opening a file
      void Prepare( ::ertool::EventData     *event_ptr,
		    ::ertool::ParticleGraph *graph_ptr,
		    bool mc);
      
      /// Function to "open" the output stream
      bool Open(const std::string& fname);
      
      /// Function to "read" the output stream
      bool WriteEntry();
      
      /// Function to "close" the output stream
      bool Close(const ::ertool::Provenance& prov);

    private:

      /// Output file handler
      TFile *_fout;
      /// Output TTree handle for EventData
      TTree *_event_tree;
      /// Output TTree handle for ParticleGraph
      TTree *_graph_tree;
      
    };
  }
}
#endif
/** @} */ // end of doxygen group 

