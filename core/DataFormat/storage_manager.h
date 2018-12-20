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
#include "Base/UtilFunc.h"
#include "data_base.h"
#include "event_ass.h"
namespace larlite {
  class trigger;
  class swtrigger;
  class potsummary;
  class event_hit;
  class hit;
  class event_track;
  class event_trackmom;
  class event_mctruth;
  class event_gtruth;
  class event_mcpart;
  class event_mctree;
  class event_user;
  class event_spacepoint;
  class event_rawdigit;
  class event_wire;
  class event_hit;
  class event_cluster;
  class event_shower;
  class event_mcshower;
  class event_mctrack;
  class event_simch;
  class event_calorimetry;
  class event_vertex;
  class event_endpoint2d;
  class event_seed;
  class event_cosmictag;
  class event_opflash;
  class event_ophit;
  class event_t0;
  class event_mcflux;
  class event_pfpart;
  class event_partid;
  class event_gtruth;
  class event_minos;
  class event_ass;
  class event_pcaxis;
  class event_flashmatch;
  class event_fifo;
  class event_opdetwaveform;
  class event_simphotons;
  class event_mucsdata;
  class event_mucsreco;
  class event_roi;
  class event_mceventweight;
  class event_auxsimch;
  //class event_chstatus;
}
#include "chstatus.h"

namespace larlite {
  /**
     \class storage_manager
     A higher level manager class to handle event-wise data and output file.
  */
  class storage_manager : public larlite_base {

  public:

    typedef std::pair<const event_ass*,AssID_t> AssInfo_t;
    typedef std::vector<AssInfo_t> AssInfoSet_t;
      
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
    void set_data_to_read(data::DataType_t const type, 
			  const std::string name)
    {
      _use_read_bool = true;
      _read_data_array[(size_t)type].insert(std::make_pair(name,true));
    }

    /// Setter to specify a certain data class to be written into output
    void set_data_to_write(data::DataType_t const type,
			   const std::string name)
    {
      _use_write_bool = true;
      _write_data_array[(size_t)type].insert(std::make_pair(name,true));
    }
    
    /// Setter for I/O mode.
    void set_io_mode(IOMode_t mode) {_mode=mode;}
    
    /// Setter for input filename
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
    bool next_event(bool store=true); 
    
    /// Move to the TTree entry with the provided index number
    bool go_to(UInt_t index, bool store=true);

    /// Move to the TTree entry with the (run_id, subrun_id, event_id)
    bool go_to( const unsigned int run_id, const unsigned int subrun_id, const unsigned int event_id );
    
    /// Close I/O file. 
    bool close(); 

    /// Event ID setter
    void set_id(const unsigned int run_id,
		const unsigned int subrun_id,
		const unsigned int event_id);

    /// Id getter
    unsigned int event_id()       const { return _event_id;       }
    unsigned int run_id()         const { return _run_id;         }
    unsigned int subrun_id()      const { return _subrun_id;      }
    unsigned int last_run_id()    const { return _last_run_id;    }
    unsigned int last_subrun_id() const { return _last_subrun_id; }

    /// Function to list product ID loaded or created
    const std::vector<larlite::product_id>& list_input_product() const { return _input_product_id; }
    
    /// Universal event data pointer getter to return event_base* pointer for specified event-data type.
    event_base* get_data(const data::DataType_t type, const std::string& name);

    /// Universal run data pointer getter to return run_base* pointer for specified run-data type w/ run number
    run_base* get_rundata(const data::RunDataType_t type, const std::string& name);

    /// Universal sub-run data pointer getter to return run_base* pointer for specified run-data type w/ run number
    subrun_base* get_subrundata(const data::SubRunDataType_t type, const std::string& name);

    /** 
	Type specific data product getter.
	Specialize the template to the data product of your choice, and it cast the
	pointer + return reference for you.
    */
    template <class T>
    T* get_data(const std::string& name)
    {
      auto type = data_type<T>();
      return (T*)(get_data(type,name));
    }

    /** 
	Type specific run data product getter.
	Specialize the template to the run data product of your choice, and it cast the
	pointer + return reference for you.
    */
    template <class T>
    T* get_rundata(const std::string& name)
    {
      auto type = rundata_type<T>();
      return (T*)(get_rundata(type,name));
    }

    /** 
	Type specific sub-run data product getter.
	Specialize the template to the sub-run data product of your choice, and it cast the
	pointer + return reference for you.
    */
    template <class T>
    T* get_subrundata(const std::string& name)
    {
      auto type = subrundata_type<T>();
      return (T*)(get_subrundata(type,name));
    }
    /*
    template <class T, class U>
    const AssInfo_t find_one_assid(const T, const U) const;
    */

#ifndef __CINT__
#ifndef __CLING__
    template <class T, class U>
    const AssInfo_t find_one_assid(const T& a, const U& b,
				   const std::string& ass_producer)
    {
      auto const& ev_ass_m = this->_ptr_data_array[data::kAssociation];
      if(ev_ass_m.find(ass_producer)==ev_ass_m.end())
	return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);	

      auto const& ev_ass = this->get_data<larlite::event_ass>(ass_producer);
      auto id = ev_ass->find_one_assid(a,b);
      if( id != kINVALID_ASS )
	return std::make_pair((const larlite::event_ass*)(ev_ass),id);

      return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);
    }

    template <class T, class U>
    const AssInfo_t find_unique_assid(const T& a, const U& b,
				      const std::string& ass_producer)
    {
      auto const& ev_ass_m = this->_ptr_data_array[data::kAssociation];
      if(ev_ass_m.find(ass_producer)==ev_ass_m.end())
	return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);	

      auto const& ev_ass = this->get_data<larlite::event_ass>(ass_producer);
      auto id = ev_ass->find_unique_assid(a,b);
      if( id != kINVALID_ASS )
	return std::make_pair((const larlite::event_ass*)(ev_ass),id);

      return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);
    }
      
    template <class T, class U>
    const AssInfoSet_t find_all_assid(const T& a, const U& b,
				      const std::string& ass_producer)
    {
      auto const& ev_ass_m = this->_ptr_data_array[data::kAssociation];
      AssInfoSet_t res;
      if(ev_ass_m.find(ass_producer)==ev_ass_m.end())
	return res;

      auto const& ev_ass = this->get_data<larlite::event_ass>(ass_producer);
      auto id_v = ev_ass->find_all_assid(a,b);
      if( id_v.size() ) {
	for(auto const& id : id_v) 
	  res.push_back((const larlite::event_ass*)(ev_ass),id);
      }
      return res;
    }

    template <class T, class U>
    const AssInfo_t find_one_assid(const T a, const U b)
    {
      auto const& ev_ass_m = this->_ptr_data_array[data::kAssociation];
      for(auto const& ev_ass_p : ev_ass_m) {
	auto res = find_one_assid(a,b,ev_ass_p.first);
	if(res.first) return res;
      }
      return std::make_pair((const larlite::event_ass*)nullptr,kINVALID_ASS);
    }

    template <class T, class U>
    const AssInfo_t find_unique_assid(const T a, const U b)
    {
      auto const& ev_ass_m = _ptr_data_array[data::kAssociation];
      event_ass* ptr=nullptr;
      AssID_t id=kINVALID_ASS;
      for(auto const& ev_ass_p : ev_ass_m) {
	auto res = find_unique_assid(a,b,ev_ass_p.first);
	if(res.first && ptr) throw DataFormatException("Association found but not unique!");
	id = res.second;
	ptr = res.first;
      }
      return std::make_pair((const larlite::event_ass*)ptr,id);
    }
    
    template <class T, class U>
    const AssInfoSet_t find_all_assid(const T a, const U b)
    {
      auto const& ev_ass_m = _ptr_data_array[data::kAssociation];
      AssInfoSet_t res;
      for(auto const& ev_ass_p : ev_ass_m) {
	auto tmp_res_v = find_all_assid(a,b,ev_ass_p.first);
	res.reserve(res.size() + tmp_res_v.size());
	for(auto const& tmp_res : tmp_res_v) res.push_back(tmp_res);
      }
      return res;
    }

    template <class T, class U>
    const AssSet_t& find_one_ass(const T a, U*& b, const std::string ass_producer="")
    {
      if(b) throw DataFormatException("Valid pointer provided (should be nullptr)!");

      auto type_b = this->data_type<U>();
      AssInfo_t ass_info;
      if(ass_producer.empty())
	ass_info = this->find_one_assid(a,type_b);
      else
	ass_info = this->find_one_assid(a,type_b,ass_producer);
      
      if(!ass_info.first) return kEMPTY_ASS;

      auto id_b = ass_info.first->association_keys(ass_info.second).second;
      b = this->get_data<U>(id_b.second);
      
      return ass_info.first->association(ass_info.second);
    }

    template <class T, class U>
    const AssSet_t& find_unique_ass(const T a, U*& b, const std::string ass_producer="")
    {
      if(b) throw DataFormatException("Valid pointer provided (should be nullptr)!");

      auto type_b = this->data_type<U>();
      AssInfo_t ass_info;
      if(ass_producer.empty())
	ass_info = this->find_unique_assid(a,type_b);
      else
	ass_info = this->find_unique_assid(a,type_b,ass_producer);
	
      if(!ass_info.first) return kEMPTY_ASS;

      auto id_b = ass_info.first->association_keys(ass_info.second).second;
      b = this->get_data<U>(id_b.second);
      
      return ass_info.first->association(ass_info.second);
    }
#endif
#endif
    
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

    /// Run data product class => enum type converter
    template <class T>
    data::RunDataType_t rundata_type() const;

    /// SubRun data product class => enum type converter
    template <class T>
    data::SubRunDataType_t subrundata_type() const;

    /// Event data product class => enum type converter
    template <class T>
    data::DataType_t data_type() const;

    /// Utility method: given a type, returns a data product name
    const std::string& product_name(data::DataType_t const type) const
    { return ::larlite::GetProductName(type); }

  private:
    static storage_manager* me; ///< shared object instance pointer
    
    void create_data_ptr(data::DataType_t const type, std::string const& name);
    void delete_data_ptr(data::DataType_t const type, std::string const& name);
    void create_rundata_ptr(data::RunDataType_t const type, std::string const& name);
    void delete_rundata_ptr(data::RunDataType_t const type, std::string const& name);
    void create_subrundata_ptr(data::SubRunDataType_t const type, std::string const& name);
    void delete_subrundata_ptr(data::SubRunDataType_t const type, std::string const& name);

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
    // number of runs read/written
    //UInt_t _nruns, _nruns_read, _nruns_written;
    // number of sub-runs read/written
    //UInt_t _nsubruns, _nsubruns_read, _nsubruns_written;
    
    unsigned int _event_id;  ///< Current event ID (used for alignment check)
    unsigned int _run_id;    ///< Current run ID (used for alignment check)
    unsigned int _subrun_id; ///< Current sub-run ID (used for alignment check)

    unsigned int _last_run_id;    ///< Last processed run ID
    unsigned int _last_subrun_id; ///< Last processed sub-run ID
    
    /// status control stamp
    IOStatus_t _status;
    
    /// I/O mode 
    IOMode_t _mode;

    /// Array of event-data product_id 
    std::vector<larlite::product_id> _input_product_id;
    
    /// Boolean to enable event alignment check
    bool _check_alignment;
    
    /// Data pointer array for reading event-wise data
    std::vector<std::map<std::string,larlite::event_base*> > _ptr_data_array;
    /// Data pointer array for reading run-wise data
    std::vector<std::map<std::string,larlite::run_base*> > _ptr_rundata_array;
    /// Data pointer array for reading subrun-wise data
    std::vector<std::map<std::string,larlite::subrun_base*> > _ptr_subrundata_array;
    
    // I/O filename
    std::string _out_fname; ///< output data file name
    std::vector<std::string> _in_fnames; ///< input data file name array
    std::string _name_in_tdirectory; ///< input files' TDirectory name (if needed) 
    std::string _name_out_tdirectory; ///< output file's TDirectory name (if needed)
     
    /// ROOT file instance
    TFile *_fout;
    
    // TTree instance
    TChain* _in_id_ch;  ///< Event ID input TTree
    TTree*  _out_id_ch; ///< Event ID output TTree
    std::vector<std::map<std::string,TChain*> > _in_ch;  ///< Array of event-data input TChains
    std::vector<std::map<std::string,TTree*>  > _out_ch; ///< Array of event-data output TChains
    std::vector<std::map<std::string,TChain*> > _in_rundata_ch;  ///< Array of run-data input TChains
    std::vector<std::map<std::string,TTree*>  > _out_rundata_ch; ///< Array of run-data output TChains
    std::vector<std::map<std::string,TChain*> > _in_subrundata_ch;  ///< Array of input sub-run data TChains
    std::vector<std::map<std::string,TTree*>  > _out_subrundata_ch; ///< Array of output sub-run data TChains

    /// Boolean for whether or not to use _out_write_bool
    bool _use_read_bool;    
    /// Boolean to record what event-data to be read out from a file
    std::vector<std::map<std::string,bool> > _read_data_array;
    /// Boolean to record what run-data to be read out from a file
    std::vector<std::map<std::string,bool> > _read_rundata_array;
    /// Boolean to record what subrun-data to be read out from a file
    std::vector<std::map<std::string,bool> > _read_subrundata_array;

    /// Boolean for whether or not to use _out_write_bool
    bool _use_write_bool;    
    /// Boolean to record what event-data to be written out from a file
    std::vector<std::map<std::string,bool> > _write_data_array;
    /// Boolean to record what run-data to be written out from a file
    std::vector<std::map<std::string,bool> > _write_rundata_array;
    /// Boolean to record what subrun-data to be written out from a file
    std::vector<std::map<std::string,bool> > _write_subrundata_array;

  };

}


#ifndef __CINT__
#ifndef __CLING__
//#include "storage_manager.template.hh"
namespace larlite {
  template<> data::DataType_t storage_manager::data_type<trigger> () const;
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
  template<> data::DataType_t storage_manager::data_type<event_t0> () const;
  template<> data::DataType_t storage_manager::data_type<event_opflash> () const;
  template<> data::DataType_t storage_manager::data_type<event_cluster> () const;
  template<> data::DataType_t storage_manager::data_type<event_seed> () const;
  template<> data::DataType_t storage_manager::data_type<event_spacepoint> () const;
  template<> data::DataType_t storage_manager::data_type<event_track> () const;
  template<> data::DataType_t storage_manager::data_type<event_trackmom> () const;
  template<> data::DataType_t storage_manager::data_type<event_shower> () const;
  template<> data::DataType_t storage_manager::data_type<event_vertex> () const;
  template<> data::DataType_t storage_manager::data_type<event_endpoint2d> () const;
  template<> data::DataType_t storage_manager::data_type<event_calorimetry> () const;
  template<> data::DataType_t storage_manager::data_type<event_partid> () const;
  template<> data::DataType_t storage_manager::data_type<event_pfpart> () const;
  template<> data::DataType_t storage_manager::data_type<event_user> () const;
  template<> data::DataType_t storage_manager::data_type<event_mctrack> () const;
  template<> data::DataType_t storage_manager::data_type<event_mctree> () const;
  template<> data::DataType_t storage_manager::data_type<event_minos> () const;
  template<> data::DataType_t storage_manager::data_type<event_cosmictag>() const;
  template<> data::DataType_t storage_manager::data_type<event_fifo>() const;
  template<> data::DataType_t storage_manager::data_type<event_ass>() const;
  template<> data::DataType_t storage_manager::data_type<event_pcaxis>() const;
  template<> data::DataType_t storage_manager::data_type<event_flashmatch>() const;
  template<> data::DataType_t storage_manager::data_type<event_opdetwaveform> () const;
  template<> data::DataType_t storage_manager::data_type<event_simphotons> () const;
  template<> data::DataType_t storage_manager::data_type<event_mucsdata> () const;
  template<> data::DataType_t storage_manager::data_type<event_mucsreco> () const;
  template<> data::DataType_t storage_manager::data_type<event_roi> () const;
  template<> data::DataType_t storage_manager::data_type<event_mceventweight> () const;
  template<> data::DataType_t storage_manager::data_type<event_auxsimch> () const;
  template<> data::DataType_t storage_manager::data_type<event_chstatus> () const;
  template<> data::DataType_t storage_manager::data_type<swtrigger> () const;
  template<> data::SubRunDataType_t storage_manager::subrundata_type<potsummary>() const;

  template<>
  const storage_manager::AssInfo_t
  storage_manager::find_one_assid(const data::DataType_t a,const data::DataType_t b);
				  
  template<>
  const storage_manager::AssInfo_t
  storage_manager::find_one_assid(const product_id& a,const data::DataType_t b);
				  
  template<>
  const storage_manager::AssInfo_t
  storage_manager::find_one_assid(const data::DataType_t a,const product_id& b);
				  
  template<>
  const storage_manager::AssInfo_t
  storage_manager::find_unique_assid(const data::DataType_t a,const data::DataType_t b);
				     
  template<>
  const storage_manager::AssInfo_t
  storage_manager::find_unique_assid(const product_id& a,const data::DataType_t b);
				     
  template<>
  const storage_manager::AssInfo_t
  storage_manager::find_unique_assid(const data::DataType_t a,const product_id& b);

  template<>
  const storage_manager::AssInfoSet_t
  storage_manager::find_all_assid(const data::DataType_t a, const data::DataType_t b);

  template<>
  const storage_manager::AssInfoSet_t
  storage_manager::find_all_assid(const product_id& a, const data::DataType_t b);

  template<>
  const storage_manager::AssInfoSet_t
  storage_manager::find_all_assid(const data::DataType_t a, const product_id& b);

}
#endif
#endif


#endif
/** @} */ // end of doxygen group larlite::storage_manager
