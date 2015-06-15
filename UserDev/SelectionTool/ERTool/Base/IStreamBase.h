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
#include "Provenance.h"

namespace ertool {
  namespace io {
    /**
       \class IStreamBase
       User defined class IStreamBase ... these comments are used to generate
       doxygen documentation!
    */
    class IStreamBase{
      
    public:
      
      /// Default constructor
      IStreamBase(const std::string& name="noname");
      
      /// Default destructor
      virtual ~IStreamBase(){}
      
      /// Name
      const std::string& Name() const;

      /// # of entries in the input file
      size_t NumEntries() const;
      
      /// Function to prepare self before opening a file
      virtual void Prepare( ::ertool::EventData     *event_ptr,
			    ::ertool::ParticleGraph *graph_ptr,
			    bool mc) = 0;
      
      /// Add an input file
      virtual void AddFile( const std::string& fname ) = 0;
      
      /// Function to "open" the input stream
      virtual bool Open( ::ertool::Provenance& in_prov) = 0;
      
      /// Function to "read" ParticleGraph
      virtual bool ReadParticleGraph(const size_t entry) = 0;
      
      /// Function to "read" EventData
      virtual bool ReadEventData(const size_t entry) = 0;
      
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

