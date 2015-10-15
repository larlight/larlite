/**
 * \file LiteStorageManager.h
 *
 * \ingroup UBDAQReader
 * 
 * \brief Class def header for a class LiteStorageManager
 *
 * @author kterao
 */

/** \addtogroup UBDAQReader

    @{*/
#ifndef LITESTORAGEMANAGER_H
#define LITESTORAGEMANAGER_H

#ifndef __CINT__
#include <boost/archive/binary_iarchive.hpp>
#include <datatypes/ub_EventRecord.h>
#endif

#include <fstream>
#include <vector>
#include <string>
#include "DataFormat/storage_manager.h"
#include "UtilFunc.h"
namespace ubdaq {
  /**
     \class LiteStorageManager
     User defined class LiteStorageManager ... these comments are used to generate
     doxygen documentation!
  */
  class LiteStorageManager{
    
  public:
    
    /// Default constructor
    LiteStorageManager();
    
    /// Default destructor
    ~LiteStorageManager(){ }

    const ::larlite::event_rawdigit&      RawDigit      () const;
    const ::larlite::event_fifo&          FIFO          () const;
    const ::larlite::event_opdetwaveform& OpDetWaveform () const;
    const ::larlite::trigger&             Trigger       () const;

    void ReadTrigger       (bool doit=true) { _read_trigger       = doit; }
    void ReadFIFO          (bool doit=true) { _read_fifo          = doit; }
    void ReadRawDigit      (bool doit=true) { _read_rawdigit      = doit; }
    void ReadOpDetWaveform (bool doit=true) { _read_opdetwaveform = doit; }

    void StoreLArLiteFile  (std::string name) { _outfile = name; }

    void AddInputFile(const std::string fname) { _input_v.push_back(fname); }

    void Reset();
    void Initialize();
    bool ProcessEvent();

    ::larlite::storage_manager getManager(){return _mgr;}


  private:
#ifndef __CINT__
    void ProcessRecord(ub_EventRecord& eventRecord);
#endif 

    bool _read_trigger;
    bool _read_fifo;
    bool _read_opdetwaveform;
    bool _read_rawdigit;

    ::larlite::event_rawdigit*      _ev_rawdigit;
    ::larlite::event_fifo*          _ev_fifo;
    ::larlite::event_opdetwaveform* _ev_opdetwaveform;
    ::larlite::trigger*             _trigger;
    bool _eof;
    std::vector<std::string> _input_v;
    int _current_input_index;
#ifndef __CINT__
    ub_EventRecord* _eventRecord;
#endif
    size_t _event_ctr;
    std::string _outfile;
    ::larlite::storage_manager _mgr;
    std::ifstream* _is;
    UBChannelMap_t fChannelMap;
    UBChannelReverseMap_t fChannelReverseMap;

  };
}

#endif
/** @} */ // end of doxygen group 

