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
#include "TChain.h"

namespace ertool {
  namespace io {
    /**
       \class ROOTInput
       User defined class ROOTInput ... these comments are used to generate
       doxygen documentation!
    */
    class ROOTInput : public IStreamBase {
      
    public:
      
      /// Default constructor
      ROOTInput(const std::string& name="ROOTInput");
      
      /// Default destructor
      ~ROOTInput(){}
      
      /// Function to prepare self before opening a file
      void Prepare( ::ertool::EventData     *event_ptr,
		    ::ertool::ParticleGraph *graph_ptr,
		    bool mc);
      
      /// Add an input file
      void AddFile( const std::string& fname );
      
      /// Function to "open" the input stream
      bool Open( ::ertool::Provenance& in_prov);

      /// Function to check if it is "open"-ed
      bool IsOpen() const;
      
      /// Function to "read" ParticleGraph
      bool ReadParticleGraph(const size_t entry);
      
      /// Function to "read" EventData
      bool ReadEventData(const size_t entry);
      
      /// Function to "close" the output stream
      bool Close();
      
    private:
      
      /// Input TChain handle for EventData
      TChain *_event_chain;
      /// Input TChain handle for ParticleGraph
      TChain *_graph_chain;
      /// Current TTree entry for EventData
      size_t _event_entry;
      /// Current TTree entry for ParticleGraph
      size_t _graph_entry;
    };
  }
}

#endif
/** @} */ // end of doxygen group 

