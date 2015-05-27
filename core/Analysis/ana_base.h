/**
 * \file ana_base.h
 *
 * \ingroup Analysis
 * 
 * \brief Base class def for all analysis classes
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/

#ifndef LARLITE_ANA_BASE_H
#define LARLITE_ANA_BASE_H

#include "Base/larlite_base.h"
#include "DataFormat/storage_manager.h"
//#include "DataFormat/DataFormat-TypeDef.h"

namespace larlite {
  /**
     \class ana_base
     A base class for analysis modules to be operated with event_waveform
     data class instance. 
  */
  class ana_base : public larlite_base{
    
  public:
    
    /// Default constructor
    ana_base() : larlite_base() { _name="ana_base"; _fout=0;}
    
    /// Default destructor
    virtual ~ana_base(){}
    
    /// Initialization method to be called before analyze any data
    virtual bool initialize(){return true;}

    /// Function to be called when new run is found
    virtual bool begin_run(storage_manager* data){return (data);}

    /// Function to be called when new sub-run is found
    virtual bool begin_subrun(storage_manager* data){return (data);}

    /// Analyze a data event-by-event  
    virtual bool analyze(storage_manager* data){return (data);}
    
    /// Finalize method to be called after all events processed.
    virtual bool finalize(){return true;}
    
    /// A setter for analysis output file poitner
    void set_output_file(TFile* fout){_fout=fout;}
    
  protected:
    
    TFile* _fout; ///< Analysis output file pointer
  };
}
#endif
  
/** @} */ // end of doxygen group 
  
