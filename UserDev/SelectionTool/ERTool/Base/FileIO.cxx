#ifndef SELECTIONTOOL_ERTOOL_FILEIO_CXX
#define SELECTIONTOOL_ERTOOL_FILEIO_CXX

#include "FileIO.h"
#include "ROOTInput.h"
#include "ROOTOutput.h"

namespace ertool {
  namespace io {
    
    FileIO::FileIO( const std::string&     proc_name,
		    const StreamType_t in_strm,
		    const StreamType_t out_strm)
      : _state     ( kINIT          )
      , _prov      ( nullptr        )
      , _io_mode   ( io::kUNDEFINED )
      , _edata     ( nullptr        )
      , _edata_mc  ( nullptr        )
      , _pgraph    ( nullptr        )
      , _pgraph_mc ( nullptr        )
      , _in_strm   ( nullptr        )
      , _out_strm  ( nullptr        )
      , _n_entries ( 0              )
      , _current_entry ( ertool::kINVALID_SIZE )
    {
      
      _prov   = new ::ertool::Provenance(proc_name);
      _edata  = new ::ertool::EventData;
      _pgraph = new ::ertool::ParticleGraph;
      _edata_mc  = new ::ertool::EventData;
      _pgraph_mc = new ::ertool::ParticleGraph;
      
      //
      // make sure in_strm and out_strm cleared up
      //
      
      // Instantiate input stream
      switch(in_strm) {
      case kEmptyStream:
	break;
      case kROOT:
	_in_strm = new ROOTInput;
      default:
	throw ertool::ERException("This mode not yet implemented!");
	break;
      }
      // Instantiate output stream
      switch(out_strm) {
      case kEmptyStream:
	break;
      case kROOT:
	_out_strm = new ROOTOutput;
      default:
	throw ertool::ERException("This mode not yet implemented!");
	break;
      }
      if(_in_strm)
	_in_strm->Prepare(_edata,_pgraph,false);
      if(_out_strm)
	_out_strm->Prepare(_edata,_pgraph,false);
    }

    EventData& FileIO::GetEventDataWriteable(bool mc)
    {
      if(_state != kOPEN)
	throw ertool::ERException("File not yet opened...");

      if(mc) return *_edata_mc;
      return *_edata;
    }
    
    ParticleGraph& FileIO::GetParticleGraphWriteable(bool mc)
    {
      if(_state != kOPEN)
	throw ertool::ERException("File not yet opened...");

      if(mc) return *_pgraph_mc;
      return *_pgraph;
    }
    
    Provenance& FileIO::GetProvenanceWriteable()
    {
      if(_state != kOPEN)
	throw ertool::ERException("File not yet opened...");

      return *_prov;
    }

    const EventData& FileIO::GetEventData(bool mc) const
    {
      if(_state != kOPEN)
	throw ertool::ERException("File not yet opened...");

      if(mc) return *_edata_mc;
      return *_edata;
    }
    
    const ParticleGraph& FileIO::GetParticleGraph(bool mc) const
    {
      if(_state != kOPEN)
	throw ertool::ERException("File not yet opened...");

      if(mc) return *_pgraph_mc;
      return *_pgraph;
    }
    
    const Provenance& FileIO::GetProvenance() const
    {
      if(_state != kOPEN)
	throw ertool::ERException("File not yet opened...");

      return *_prov;
    }

    void FileIO::AddInputFile(const std::string& fname)
    {
      if(_state == kOPEN)
	throw ::ertool::ERException("Cannot add input file after OPEN-ed!");
      if(fname.empty()) return;
      if(!_in_strm) 
	throw ::ertool::ERException("<<AddInputFile>> called but input stream does not exist!");
      
      switch(_io_mode) {
      case kUNDEFINED:
	_io_mode = io::kREAD; break;
      case kWRITE:
	_io_mode = io::kBOTH; break;
      case kREAD:
      case kBOTH:
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
    
    void FileIO::SetOutputFile(const std::string& fname)
    {
      if(_state == kOPEN)
	throw ::ertool::ERException("Cannot set output file after OPEN-ed!");
      if(fname.empty()) return;
      if(!_out_strm) 
	throw ::ertool::ERException("<<SetOutputFile>> called but output stream does not exist!");      

      switch(_io_mode) {
      case kUNDEFINED:
	_io_mode = kWRITE; break;
      case kREAD:
	_io_mode = kBOTH; break;
      case kWRITE:
      case kBOTH:
	break;
      }

      const std::string key("OUTPUT_FILE_NAME");
      if(_prov->HasInfo(key)) _prov->SetInfo(key,fname);
      else _prov->AddInfo(key,fname);
      
      _out_file_name = fname;
      
    }

    /// Open input/output file(s)
    bool FileIO::Open()
    {
      if(_state != kINIT)
	throw ::ertool::ERException("Cannot OPEN if aleady OPEN-ed!");
      
      if( !_in_strm && !_out_strm )
	throw ::ertool::ERException("Neither of I/O stream handler set...");

      bool status = true;
      
      for(auto const& fname : _in_file_name_v) {
	
	if( !_in_strm )
	  throw ::ertool::ERException("Input files provided but no input stream handler provided!");
	
	_in_strm->AddFile(fname);
      }
      
      if( _out_file_name.empty() &&  _out_strm )
	
	throw ::ertool::ERException("Output file not provided but output stream handler provided!");

      if( !_out_file_name.empty() && !_out_strm )
	
	throw ::ertool::ERException("Output file provided but output stream handler not provided!");

      if( _in_strm  ) {

	status = status && _in_strm->Open(*_prov);

	_n_entries = _in_strm->NumEntries();

      }
      
      if( _out_strm )
	
	status = status && _out_strm->Open(_out_file_name);
      
      return status;
    }
  
    /// Read a specified entry (event) 
    bool FileIO::ReadEntry(size_t entry)
    {
      if( _state != kOPEN )
	throw ::ertool::ERException("Cannot read entry before OPEN a file");
      if( !_in_strm )
	throw ::ertool::ERException("Input stream does not exist!");

      bool status = true;
      switch(_io_mode) {
      case kBOTH:
	status = status && _in_strm->ReadParticleGraph(entry);
	status = status && _in_strm->ReadEventData(entry);
      case kREAD:
	_current_entry = entry;
	break;
      default:
	throw ::ertool::ERException("There is no input stream!");
	break;
      }
      return status;
    }
    
    /// Write the current entry (event)
    bool FileIO::WriteEntry()
    {
      if( _state != kOPEN )
	throw ::ertool::ERException("Cannot write entry before OPEN a file");
      if( !_out_strm )
	throw ::ertool::ERException("Output stream does not exist!");

      bool status = true;
      switch(_io_mode) {
      case kWRITE:
	++_n_entries;
      case kBOTH:
	status = status && _out_strm->WriteEntry();
	break;
      default:
	throw ::ertool::ERException("There is no output stream!");
	break;
      }

      return status;
    }
    
    /// Close input/output file(s)
    bool FileIO::Close()
    {
      if( _state != kOPEN )
	throw ::ertool::ERException("Cannot close before OPEN a file");
      
      bool status = true;
      if( _in_strm  ) status = status && _in_strm->Close();
      if( _out_strm ) status = status && _out_strm->Close(*_prov);
      _state = kINIT;
      return status;
    }
  }
}

#endif
