/**
 * \file ana_processor.h
 *
 * \ingroup Analysis
 * 
 * \brief A batch processor class for arbitrary number of analysis modules
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/
#ifndef LARLITE_ANA_PROCESSOR_H
#define LARLITE_ANA_PROCESSOR_H

#include <vector>
#include "ana_base.h"

namespace larlite {
  /**
     \class ana_processor
     A class to be loaded with arbitrary number of ana_base inherited analysis
     modules. This class process data by applying loaded analysis modules in
     consecutive order.
  */
  class ana_processor : public larlite_base{
    
  public:
    
    /// Process flag enum
    enum ProcessFlag_t{
      kINIT,       ///< Process is @ the initialization stage
      kREADY,      ///< Process is ready to start data processing
      kPROCESSING, ///< Process is processing data
      kFINISHED    ///< Process has finished processing allevents.
    };
    
    /// Default constructor
    ana_processor();
    
    /// Default destructor
    ~ana_processor(){}
    
    /// Override a method to set verbosity
    /**
       It changes the verbosity level of not just this class but also owned
       other class instances.
    */
    virtual void set_verbosity(msg::Level level);
    
    /// Setter for an input data file name
    void add_input_file(std::string name){_storage->add_in_filename(name);}

    /// Setter for an input ROOT TDirectory name
    void set_input_rootdir(std::string name) {_storage->set_in_rootdir(name);}

    /// Setter for an input ROOT TDirectory name
    void set_output_rootdir(std::string name) {_storage->set_out_rootdir(name);}

    /// Setter for an input ROOT TDirectory name
    void set_rootdir(std::string name) {_storage->set_rootdir(name);}
    
    /// Setter for an output data file name
    void set_output_file(std::string name){_storage->set_out_filename(name);}
    
    /// Setter for an analysis output root file name
    void set_ana_output_file(std::string name){_ofile_name=name;}
    
    /// Setter for I/O mode ... READ or WRITE or BOTH 
    void set_io_mode(storage_manager::IOMode_t mode){_storage->set_io_mode(mode);}
    
    /// Setter to specify a certain data class to be read from input
    void set_data_to_read(data::DataType_t const type, 
			  const std::string name)
    { _storage->set_data_to_read(type,name); }

    /// Setter to specify a certain data class to be written into output
    void set_data_to_write(data::DataType_t const type, 
			   const std::string name)
    { _storage->set_data_to_write(type,name); }    

    /// alignment check switch
    void enable_event_alignment(bool doit=true)
    { _storage->enable_event_alignment(doit); }
    
    /// Getter of running analysis status
    Bool_t get_ana_status(ana_base* ptr) const;
    
    /// A method to run a batch process 
    Bool_t run(UInt_t start_index=0, UInt_t nevents=0);
    
    /// A method to process just one event.
    Bool_t process_event(UInt_t index=data::kINVALID_UINT);
    
    /// A method to append analysis class instance. Returns index number.
    Size_t add_process(ana_base* ana,bool filter=false)
    {
      _ana_index.insert(std::make_pair(ana,_analyzers.size()));
      _analyzers.push_back(ana);
      _filter_marker_v.push_back(filter);
      return (*_ana_index.find(ana)).second;
    }
    
    /// A method to inquir attached analysis class instance.
    ana_base* get_process(Size_t loc){return (_analyzers.size() > loc) ? _analyzers[loc] : 0;}
    
    /// A method to inquire the process status
    ProcessFlag_t get_process_status() {return _process;}

    /// Setter to enable filtering mode
    void enable_filter(bool doit=true) { _filter_enable = doit; }

    /// A method to reset members
    void reset();
    
    /// A method to initialize and prepare for running analysis
    Bool_t initialize();
    
    /// A method to finalize data processing
    Bool_t finalize();

  private:
    std::vector<ana_base*>   _analyzers;  ///< A vector of analysis modules
    std::vector<bool>        _ana_status; ///< A vector of analysis modules' status
    std::vector<bool>   _filter_marker_v; ///< A vector to mark specific analysis unit as a filter
    std::map<ana_base*,size_t> _ana_index; ///< A map of analysis module status
    
    ProcessFlag_t _process;    ///< Processing status flag
    UInt_t _nevents;           ///< Number of events being processed
    UInt_t _index;             ///< Index of currently processing event
    std::string _ofile_name;   ///< Output file name
    TFile*   _fout;            ///< Output file pointer
    storage_manager* _storage; ///< Storage manager pointer

    bool _filter_enable;
    bool _ana_unit_status;
    
  };
}
#endif

/** @} */ // end of doxygen group 
