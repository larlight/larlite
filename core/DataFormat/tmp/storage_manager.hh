/**
 * \file storage_manager.hh
 *
 * \ingroup DataFormat
 * 
 * \brief Higher elve data handling class def header file.
 *
 * @author Kazu - Nevis 2013
 */
/** \addtogroup DataFormat

    @{*/

#ifndef STORAGE_MANAGER_HH
#define STORAGE_MANAGER_HH

#include <TFile.h>
#include <TChain.h>
#include "data_base.hh"
#include "larlight_base.hh"
#include "track.hh"
#include "mctruth.hh"
#include "user_info.hh"
#include "spacepoint.hh"
#include "pmtfifo.hh"
#include "tpcfifo.hh"
#include "trigger.hh"
#include "wire.hh"
#include "hit.hh"
#include "cluster.hh"
#include "shower.hh"
#include "mcshower.hh"
#include "simch.hh"
#include "calorimetry.hh"
#include "pulse.hh"
#include "vertex.hh"
#include "endpoint2d.hh"
#include "cosmictag.hh"
#include "opflash.hh"
#include "ophit.hh"
namespace larlight {
  /**
     \class storage_manager
     A higher level manager class to handle event-wise data and output file.
  */
  class storage_manager : public larlight_base {
    
  public:
    
    /// Defines I/O mode ... currently only read OR write.
    enum MODE{
      READ,       ///< I/O mode: READ
      WRITE,      ///< I/O mode: WRITE
      BOTH,       ///< I/O mode both read & write
      UNDEFINED   ///< I/O mode: undefined ...
    };
    
    /// Defines I/O process status flags
    enum STATUS{
      INIT,       ///< I/O operation status: initiated
      OPENED,     ///< I/O file opened
      READY_IO,   ///< All pre-I/O process finished (making TTrees, etc.)
      OPERATING,  ///< Reading/Writing events
      CLOSED      ///< I/O finished and file closed already
    };
    
    /// Default constructor. Allow user to specify the I/O mode here.
    storage_manager(MODE mode=UNDEFINED);
    
    /// Default destructor.
    virtual ~storage_manager(){}
    
    /// Setter to specify a certain data class to be read from input
    void set_data_to_read(DATA::DATA_TYPE type,bool read=true)
    {_read_data_array[type]=read;}
    
    /// Setter for I/O mode.
    void set_io_mode(MODE mode)         {_mode=mode;}
    
    /// Setter for input filename
    //void set_in_filename(std::string name) {_in_fname=name;}
    void add_in_filename(std::string name) {_in_fnames.push_back(name);}
    
    /// Setter for input file's TDirectory name
    void set_in_rootdir(std::string name){ _name_in_tdirectory=name;}

    /// Setter for output file's TDirectory name
    void set_out_rootdir(std::string name) { _name_out_tdirectory=name;}

    /// Setter for both (in/out) file's TDirectory name
    void set_rootdir(std::string name) { _name_in_tdirectory=name; _name_out_tdirectory=name; }
    
    /// Setter for output filename
    void set_out_filename(std::string name) {_out_fname=name;}

    /// Setter to disable event alignment check
    void enable_event_alignment(bool doit=true) {_check_alignment=doit;}
    
    /// Getter for I/O mode.
    MODE io_mode() const         {return _mode;}
    
    /// Getter for an array of input files set by the user
    std::vector<std::string> input_filename() const {return _in_fnames;}
    
    /// Getter for I/O file name.
    std::string output_filename() const {return _out_fname;}
    
    /// Getter for process status stamp. See storage_manager::STATUS
    STATUS status() const        {return _status;}
    
    /// Reset variables and I/O process to prepare for a new I/O.
    void reset();
    
    /// Open I/O file. Return true upon success.
    bool open();  
    
    /// Utility boolean to check if file is opened.
    bool is_open();
    
    /// Utility boolean to check if file is opened and ready for I/O
    bool is_ready_io();
    
    /// Stores an event to the output in WRITE mode, and loads an event to local memory in READ mode.
    bool next_event(); 
    
    /// Move to the TTree entry with the provided index number
    bool go_to(UInt_t index);
    
    /// Close I/O file. 
    bool close(); 
    
    /** Universal data pointer getter to return data_base* pointer for specified data type.
	A user then cast the pointer type to whatever the relevant data class pointer type to access members.
    */
    event_base* get_data(DATA::DATA_TYPE type);
    
    /// What data is available?
    std::vector<larlight::DATA::DATA_TYPE> list_data_types();
    
    /// Getter for a shared object instance pointer. Not limited to be a singleton.
    static storage_manager* get() 
    { if(!me) me= new storage_manager(); return me; }
    
    /// killer for a shared object instance.
    static void kill() 
    { if(me) delete me;}
    
    /// Getter for a number of events in the file.
    inline UInt_t get_entries() const {return _nevents;}
    
    /// Getter for a TTree index
    inline UInt_t get_index()   const {return _index;}
    
    /// Getter for a counter of read-out events
    inline UInt_t get_entries_read() const {return _nevents_read;}
    
    /// Getter for a counter of written-out events
    inline UInt_t get_entires_written() const {return _nevents_written;}
    
  private:
    
    static storage_manager* me; ///< shared object instance pointer
    
    /// 
    void create_data_ptr(DATA::DATA_TYPE type);
    
    void delete_data_ptr(DATA::DATA_TYPE type);
    
    /// Load next event from imput  
    bool read_event();
    
    /// Write the current event into output
    bool write_event();
    
    /// Called upon opening a file to prepare read/write events in TTree.
    bool prepare_tree();
    
    /// tree index marker
    UInt_t _index;
    
    /// number of events read/written
    UInt_t _nevents, _nevents_read, _nevents_written;

    /// Current event ID (used for alignment check)
    Int_t _current_event_id;
    
    /// status control stamp
    STATUS _status;
    
    /// I/O mode 
    MODE _mode;
    
    /// Boolean to record what data to be read out from a file
    bool _read_data_array[DATA::DATA_TYPE_MAX];
    
    /// Boolean to record what data to be written out from a file
    bool _write_data_array[DATA::DATA_TYPE_MAX];

    /// Boolean to enable event alignment check
    bool _check_alignment;
    
    /// Data pointer array for reading
    event_base *_ptr_data_array[DATA::DATA_TYPE_MAX];
    
    /// I/O filename
    std::string _out_fname;
    std::vector<std::string> _in_fnames;
    std::string _name_in_tdirectory;
    std::string _name_out_tdirectory;
    
    /// TTree name
    const std::string _treename;
    
    /// ROOT file instance
    //TFile *_fin, *_fout;
    TFile *_fout;
    
    /// TTree instance
    //TTree *_in_tree, *_out_tree;  
    TChain *_in_ch[DATA::DATA_TYPE_MAX];
    TTree  *_out_ch[DATA::DATA_TYPE_MAX];
    
  };
}
#endif
/** @} */ // end of doxygen group storage_manager
