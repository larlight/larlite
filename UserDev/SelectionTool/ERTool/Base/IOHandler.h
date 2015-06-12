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
  /**
     \class IOHandler
     User defined class IOHandler ... these comments are used to generate
     doxygen documentation!
  */
  class IOHandler{
    
  public:
    
    /// Default constructor
    IOHandler( const std::string&     proc_name = "no_name",
	       const io::StreamType_t in_strm   = io::kEmptyStream,
	       const io::StreamType_t out_strm  = io::kEmptyStream);
    
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

    /// Write the current entry (event)
    bool WriteEntry();

    /// Close input/output file(s)
    bool Close();

  protected:

    enum _State_t {
      kINIT,
      kOPEN
    };
    /// Process status (internal)
    _State_t _state;
    /// Process identifier name
    Provenance *_prov;
    /// I/O mode
    io::Mode_t _io_mode;
    /// Input file name (multiple)
    std::vector<std::string> _in_file_name_v;
    /// Output file name
    std::string _out_file_name;
    /// Direct EventData handles
    EventData *_edata, *_edata_mc;
    /// Direct ParticleGraph handles
    ParticleGraph *_pgraph, *_pgraph_mc;
    /// Input stream handler
    IStreamBase* _in_strm;
    /// Output stream handler
    OStreamBase* _out_strm;
  };
}

#endif
/** @} */ // end of doxygen group 

