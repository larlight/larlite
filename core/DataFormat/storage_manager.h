/**
 * \file storage_manager.h
 *
 * \ingroup DataFormat
 * 
 * \brief Higher elve data handling class def header file.
 *
 * @author Kazu - Nevis 2013
 */
/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_STORAGE_MANAGER_H
#define LARLITE_STORAGE_MANAGER_H

#include <TFile.h>
#include <TChain.h>
#include <TError.h>
#include "Base/larlite_base.h"
#include "data_base.h"
#include "potsummary.h"
//#include "larlite_association.h"
#include "hit.h"
#include "track.h"
#include "mctruth.h"
#include "user_info.h"
#include "spacepoint.h"
#include "rawdigit.h"
#include "wire.h"
#include "hit.h"
#include "cluster.h"
#include "shower.h"
#include "mcshower.h"
#include "simch.h"
#include "calorimetry.h"
#include "vertex.h"
#include "endpoint2d.h"
#include "seed.h"
#include "cosmictag.h"
#include "opflash.h"
#include "ophit.h"
#include "mcflux.h"
#include "pfpart.h"
#include "partid.h"
#include "gtruth.h"
namespace larlite {
  /**
     \class storage_manager
     A higher level manager class to handle event-wise data and output file.
  */
  class storage_manager : public larlite_base {
    
  public:
    
    /// Defines I/O mode ... currently only read OR write.
    enum IOMode_t{
      kREAD,       ///< I/O mode: READ
      kWRITE,      ///< I/O mode: WRITE
      kBOTH,       ///< I/O mode both read & write
      kUNDEFINED   ///< I/O mode: undefined ...
    };
    
    /// Defines I/O process status flags
    enum IOStatus_t{
      kINIT,       ///< I/O operation status: initiated
      kOPENED,     ///< I/O file opened
      kREADY_IO,   ///< All pre-I/O process finished (making TTrees, etc.)
      kOPERATING,  ///< Reading/Writing events
      kCLOSED      ///< I/O finished and file closed already
    };
    
    /// Default constructor. Allow user to specify the I/O mode here.
    storage_manager(IOMode_t mode=kUNDEFINED);
    
    /// Default destructor.
    virtual ~storage_manager(){}
    
    /// Setter to specify a certain data class to be read from input
    //void set_data_to_read(data::DataType_t const type, bool read=true)
    //{ _read_data_array[type]=read; }
    
    /// Setter for I/O mode.
    void set_io_mode(IOMode_t mode)         {_mode=mode;}
    
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
    IOMode_t io_mode() const         {return _mode;}
    
    /// Getter for an array of input files set by the user
    std::vector<std::string> input_filename() const {return _in_fnames;}
    
    /// Getter for I/O file name.
    std::string output_filename() const {return _out_fname;}
    
    /// Getter for process status stamp. See storage_manager::STATUS
    IOStatus_t status() const        {return _status;}
    
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

    /// Function to list product ID loaded or created
    const std::vector<larlite::product_id>& list_input_product() const { return _input_product_id; }
    
    /// Universal data pointer getter to return data_base* pointer for specified data type.
    event_base* get_data(data::DataType_t const type, std::string const name);

    /** 
	Type specific data product getter.
	Specialize the template to the data product of your choice, and it cast the
	pointer + return reference for you.
    */
    //T* get_data(std::string const name);
    template <class T>
    T* get_data(std::string const name)
    {
      auto type = data_type<T>();
      return (T*)(get_data(type,name));
    }
    
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
    inline UInt_t get_entries_written() const {return _nevents_written;}

    /// Data product class => enum type converter
    template <class T>
    data::DataType_t data_type() const;

    const std::string& product_name(data::DataType_t const type) const
    { return data::kDATA_TREE_NAME[type]; }
  private:
    
    static storage_manager* me; ///< shared object instance pointer
    
    /// 
    void create_data_ptr(data::DataType_t const type, std::string const& name);
    
    void delete_data_ptr(data::DataType_t const type, std::string const& name);
    
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
    IOStatus_t _status;
    
    /// I/O mode 
    IOMode_t _mode;

    /// Array of product_id 
    std::vector<larlite::product_id> _input_product_id;
    
    /// Boolean to record what data to be read out from a file
    std::vector<std::map<std::string,bool> > _read_data_array;
    
    /// Boolean to record what data to be written out from a file
    std::vector<std::map<std::string,bool> > _write_data_array;

    /// Boolean to enable event alignment check
    bool _check_alignment;
    
    /// Data pointer array for reading
    //std::vector<std::map<std::string,event_base*> > _ptr_data_array;
    std::vector<std::map<std::string,larlite::event_base*> > _ptr_data_array;
    
    /// I/O filename
    std::string _out_fname;
    std::vector<std::string> _in_fnames;
    std::string _name_in_tdirectory;
    std::string _name_out_tdirectory;
    
    /// ROOT file instance
    TFile *_fout;
    
    /// TTree instance
    std::vector<std::map<std::string,TChain*> > _in_ch;
    std::vector<std::map<std::string,TTree*>  > _out_ch;
  };

}


#ifndef __CINT__
//#include "storage_manager.template.hh"
namespace larlite {
  template<> data::DataType_t storage_manager::data_type<potsummary> () const;
  template<> data::DataType_t storage_manager::data_type<event_gtruth> () const;
  template<> data::DataType_t storage_manager::data_type<event_mctruth> () const;
  template<> data::DataType_t storage_manager::data_type<event_mcpart> () const;
  template<> data::DataType_t storage_manager::data_type<event_mcflux> () const;
  template<> data::DataType_t storage_manager::data_type<event_simch> () const;
  template<> data::DataType_t storage_manager::data_type<event_mcshower> () const;
  template<> data::DataType_t storage_manager::data_type<event_rawdigit> () const;
  template<> data::DataType_t storage_manager::data_type<event_wire> () const;
  template<> data::DataType_t storage_manager::data_type<event_hit> () const;
  template<> data::DataType_t storage_manager::data_type<event_ophit> () const;
  template<> data::DataType_t storage_manager::data_type<event_opflash> () const;
  template<> data::DataType_t storage_manager::data_type<event_cluster> () const;
  template<> data::DataType_t storage_manager::data_type<event_seed> () const;
  template<> data::DataType_t storage_manager::data_type<event_spacepoint> () const;
  template<> data::DataType_t storage_manager::data_type<event_track> () const;
  template<> data::DataType_t storage_manager::data_type<event_shower> () const;
  template<> data::DataType_t storage_manager::data_type<event_vertex> () const;
  template<> data::DataType_t storage_manager::data_type<event_endpoint2d> () const;
  template<> data::DataType_t storage_manager::data_type<event_calorimetry> () const;
  template<> data::DataType_t storage_manager::data_type<event_partid> () const;
  template<> data::DataType_t storage_manager::data_type<event_pfpart> () const;
  template<> data::DataType_t storage_manager::data_type<event_user> () const;
}
#endif

#endif
/** @} */ // end of doxygen group larlite::storage_manager
