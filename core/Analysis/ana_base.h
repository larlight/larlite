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

#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TGraphErrors.h>

#include "Base/larlite_base.h"
#include "DataFormat/DataFormat-TypeDef.h"

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
    virtual Bool_t initialize(){return true;}
    
    /// Analyze a data event-by-event  
    virtual Bool_t analyze(storage_manager* data){return (data);}
    
    /// Finalize method to be called after all events processed.
    virtual Bool_t finalize(){return true;}
    
    /// A setter for analysis output file poitner
    void set_output_file(TFile* fout){_fout=fout;}
    
  protected:
    
    TFile* _fout; ///< Analysis output file pointer
  };
}
#endif
  
/** @} */ // end of doxygen group 
  
