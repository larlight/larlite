/**
 * \file IOHandler.h
 *
 * \ingroup Base
 * 
 * \brief Class def header for a class IOHandler
 *
 * @author kazuhiro
 */

/** \addtogroup Base

    @{*/
#ifndef SELECTIONTOOL_ERTOOL_IOHANDLER_H
#define SELECTIONTOOL_ERTOOL_IOHANDLER_H

#include "IStreamBase.h"
#include "OStreamBase.h"

namespace ertool {
  class Manager;
  namespace io {
    /**
       \class IOHandler
       User defined class IOHandler ... these comments are used to generate
       doxygen documentation!
    */
    class IOHandler{
      friend class ::ertool::Manager;
    public:
      
      /// Default constructor
      IOHandler( const std::string& proc_name = "no_name",
		 const StreamType_t in_strm   = kEmptyStream,
		 const StreamType_t out_strm  = kEmptyStream);
      
      /// Default destructor
      ~IOHandler(){}
      
      /// Input file name adder
      void AddInputFile( const std::string& fname );
      
      /// Output file name setter
      void SetOutputFile( const std::string& fname );

      /// Open input/output file(s)
      bool Open();
      
      /// Read a specified entry (event) 
      bool ReadEntry(size_t entry);

      /// EventData accessor
      const EventData& GetEventData(bool mc=false) const;

      /// ParticleGraph accessor
      const ParticleGraph& GetParticleGraph(bool mc=false) const;

      /// Provenance accessor
      const Provenance& GetProvenance() const;
      
      /// Write the current entry (event)
      bool WriteEntry();
      
      /// Close input/output file(s)
      bool Close();

      /// Clear event-wise data contents
      void ClearData();
      
    protected:

      /// EventData accessor
      EventData& GetEventDataWriteable(bool mc=false);

      /// ParticleGraph accessor
      ParticleGraph& GetParticleGraphWriteable(bool mc=false);

      /// Provenance accessor
      Provenance& GetProvenanceWriteable();
      
      enum _State_t {
	kINIT,
	kOPEN
      };
      /// Process status (internal)
      _State_t _state;
      /// Process identifier name
      ::ertool::Provenance *_prov;
      /// I/O mode
      Mode_t _io_mode;
      /// Input file name (multiple)
      std::vector<std::string> _in_file_name_v;
      /// Output file name
      std::string _out_file_name;
      /// Direct EventData handles
      ::ertool::EventData *_edata, *_edata_mc;
      /// Direct ParticleGraph handles
      ::ertool::ParticleGraph *_pgraph, *_pgraph_mc;
      /// Input stream handler
      ::ertool::io::IStreamBase* _in_strm;
      /// Output stream handler
      ::ertool::io::OStreamBase* _out_strm;
      /// # entries from input stream
      size_t _n_entries;
      /// "current" entry number
      size_t _current_entry;
    };
  }
}

#endif
/** @} */ // end of doxygen group 

