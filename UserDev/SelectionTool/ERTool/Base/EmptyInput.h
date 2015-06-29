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
      const std::string& Name() const;

      /// # of entries in the input file
      size_t NumEntries() const;
      
      /// Function to prepare self before opening a file
      virtual void Prepare( ::ertool::EventData     *event_ptr,
			    ::ertool::ParticleGraph *graph_ptr,
			    bool mc);
      
      /// Add an input file
      virtual void AddFile( const std::string& fname );
      
      /// Function to "open" the input stream
      virtual bool Open( ::ertool::Provenance& in_prov);
      
      /// Function to "read" ParticleGraph
      virtual bool ReadParticleGraph(const size_t entry);
      
      /// Function to "read" EventData
      virtual bool ReadEventData(const size_t entry);
      
      /// Function to "close" the output stream
      virtual bool Close() = 0;

    protected:
      /// # entries
      size_t _n_entries;
    private:
      /// Name of this instance
      std::string _name;
    };
  }
}

#endif
/** @} */ // end of doxygen group 

