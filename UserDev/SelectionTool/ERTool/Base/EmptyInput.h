/**
 * \file EmptyInput.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class EmptyInput
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_EMPTYINPUT_H
#define SELECTIONTOOL_ERTOOL_EMPTYINPUT_H

#include "IStreamBase.h"

namespace ertool {
  namespace io {
    /**
       \class EmptyInput
       User defined class EmptyInput ... these comments are used to generate
       doxygen documentation!
    */
    class EmptyInput : public IStreamBase {
      
    public:
      
      /// Default constructor
      EmptyInput(const std::string& name="noname");
      
      /// Default destructor
      virtual ~EmptyInput(){}
      
      /// Name

      //const std::string& Name() const;

      /// # of entries in the input file
      size_t NumEntries() const;
      
      /// Function to prepare self before opening a file
      void Prepare( ::ertool::EventData     *event_ptr,
			    ::ertool::ParticleGraph *graph_ptr,
			    bool mc);
      
      /// Add an input file
      void AddFile( const std::string& fname );
      
      /// Function to "open" the input stream
      bool Open( ::ertool::Provenance& in_prov);
      
      /// Function to "read" ParticleGraph
      bool ReadParticleGraph(const size_t entry);
      
      /// Function to "read" EventData
      bool ReadEventData(const size_t entry);
      
      /// Function to "close" the output stream
      bool Close();

      /// Function to set event id, run and subrun
      void SetID(unsigned int evID,
		 unsigned int runID,
		 unsigned int subrunID);

      /// Function to add input data product: Shower
      NodeID_t Add(const ertool::Shower& obj,
		   const ertool::RecoInputID_t& input_id,
		   const bool mc=false);
      
      /// Function to add input data product: Track
      NodeID_t Add(const ertool::Track&  obj,
		   const ertool::RecoInputID_t& input_id,
		   const bool mc=false);
#ifndef __CINT__
      /// Function to add input data product: Shower
      NodeID_t Emplace(const ertool::Shower&& obj, const ertool::RecoInputID_t&& input_id, const bool mc=false);
      /// Function to add input data product: Track
      NodeID_t Emplace(const ertool::Track&&  obj, const ertool::RecoInputID_t&& input_id, const bool mc=false);
#endif

      /// ParticleGraph accessor
      ::ertool::ParticleGraph& GetParticleGraphWriteable(bool mc=false);

    private:
      
      ::ertool::ParticleGraph *_graph;
      ::ertool::ParticleGraph *_mc_graph;
      ::ertool::EventData *_data;
      ::ertool::EventData *_mc_data;
      
    };
  }
}

#endif
/** @} */ // end of doxygen group 

