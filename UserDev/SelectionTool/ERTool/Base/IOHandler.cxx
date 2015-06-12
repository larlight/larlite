#ifndef SELECTIONTOOL_ERTOOL_IOHANDLER_CXX
#define SELECTIONTOOL_ERTOOL_IOHANDLER_CXX

#include "IOHandler.h"
#include "ROOTInput.h"

namespace ertool {

  IOHandler::IOHandler( const std::string&     proc_name,
		        const io::StreamType_t in_strm,
		        const io::StreamType_t out_strm)
    : _state     ( kINIT          )
    , _prov      ( nullptr        )
    , _io_mode   ( io::kUNDEFINED )
    , _edata     ( nullptr        )
    , _edata_mc  ( nullptr        )
    , _pgraph    ( nullptr        )
    , _pgraph_mc ( nullptr        )
    , _in_strm   ( nullptr        )
    , _out_strm  ( nullptr        )
  {

    _prov   = new Provenance(proc_name);
    _edata  = new EventData;
    _pgraph = new ParticleGraph;
    _edata_mc  = new EventData;
    _pgraph_mc = new ParticleGraph;

    //
    // make sure in_strm and out_strm cleared up
    //

    // Instantiate input stream
    switch(in_strm) {
    case io::kEmptyStream:
      break;
    case io::kROOT:
      //_in_strm = new ROOTInput;
      break;
    }
    // Instantiate output stream
    switch(out_strm) {
    case io::kEmptyStream:
      break;
    case io::kROOT:
      //_out_strm = new ROOTOutput;
      break;
    }

  }

  void IOHandler::AddInputFile(const std::string& fname)
  {
    if(_state == kOPEN) throw ERException("Cannot add input file after OPEN-ed!");
    if(fname.empty()) return;
    
    switch(_io_mode) {
    case io::kUNDEFINED:
      _io_mode = io::kREAD; break;
    case io::kWRITE:
      _io_mode = io::kBOTH; break;
    case io::kREAD:
    case io::kBOTH:
      break;
    }
    for(auto const& f : _in_file_name_v)
      if(f == fname) return;

    const std::string key("INPUT_FILE_LIST");
    if(_prov->HasInfo(key)) {

      std::string files = (*(_prov->Info().find(key))).second;

      files += ":" + fname;

      _prov->SetInfo(key,fname);
      
    }else

      _prov->AddInfo(key,fname);
      
      
    _in_file_name_v.push_back(fname);
  }
  
  void IOHandler::SetOutputFile(const std::string& fname)
  {
    if(_state == kOPEN) throw ERException("Cannot set output file after OPEN-ed!");
    if(fname.empty()) return;

    switch(_io_mode) {
    case io::kUNDEFINED:
      _io_mode = io::kWRITE; break;
    case io::kREAD:
      _io_mode = io::kBOTH; break;
    case io::kWRITE:
    case io::kBOTH:
      break;
    }

    const std::string key("OUTPUT_FILE_NAME");
    if(_prov->HasInfo(key)) _prov->SetInfo(key,fname);
    else _prov->AddInfo(key,fname);
    
    _out_file_name = fname;
    
  }

  /// Open input/output file(s)
  bool IOHandler::Open()
  {
    if(_state != kINIT) throw ERException("Cannot OPEN if aleady OPEN-ed!");

    if( !_in_strm && !_out_strm ) throw ERException("Neither of I/O stream handler set...");

    bool status = true;

    for(auto const& fname : _in_file_name_v) {

      if( !_in_strm ) throw ERException("Input files provided but no input stream handler provided!");

      _in_strm->AddFile(fname);
    }

    if( _out_file_name.empty() &&  _out_strm )

      throw ERException("Output file not provided but output stream handler provided!");

    if( !_out_file_name.empty() && !_out_strm )

      throw ERException("Output file provided but output stream handler not provided!");

    if( _in_strm  ) status = status && _in_strm->Open();

    if( _out_strm ) {

      _out_strm->SetFile(_out_file_name);

      status = status && _out_strm->Open();

    }
    
    return status;
  }
  
  /// Read a specified entry (event) 
  bool IOHandler::ReadEntry(size_t entry)
  {
    if( _state != kOPEN ) throw ERException("Cannot read entry before OPEN a file");
    bool status = true;
    if( _in_strm )
      status = _in_strm->Read(entry);
    return status;
  }
  
  /// Write the current entry (event)
  bool IOHandler::WriteEntry()
  {
    if( _state != kOPEN ) throw ERException("Cannot write entry before OPEN a file");
    bool status = true;
    if( _out_strm )
      status = _out_strm->Write();
    return status;
  }
  
  /// Close input/output file(s)
  bool IOHandler::Close()
  {
    if( _state != kOPEN ) throw ERException("Cannot close before OPEN a file");
    bool status = true;
    if( _in_strm  ) status = status && _in_strm->Close();
    if( _out_strm ) status = status && _out_strm->Close(_prov);
    _state = kINIT;
    return status;
  }
  
}

#endif
