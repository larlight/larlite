/**
 * \file ExampleSPFit.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class ExampleSPFit
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef LARLITE_EXAMPLESPFIT_H
#define LARLITE_EXAMPLESPFIT_H

#include "Analysis/ana_base.h"
#include "SPFilterBase.h"
#include "SPAlgoBase.h"

namespace larlite {
  /**
     \class ExampleSPFit
     User custom analysis class made by kazuhiro
   */
  class ExampleSPFit : public ana_base{
  
  public:

    /// Default constructor
    ExampleSPFit() 
      : _filter (nullptr)
      , _algo   (nullptr)
    { _name="ExampleSPFit"; _fout=0; _shower_producer=""; };

    /// Default destructor
    virtual ~ExampleSPFit(){};

    /** IMPLEMENT in ExampleSPFit.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ExampleSPFit.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ExampleSPFit.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetFilter(::sptool::SPFilterBase* f) { _filter = f; }
    
    void SetAlgo(::sptool::SPAlgoBase* a) { _algo = a; }

    bool _mc;

    std::string _shower_producer;

  protected:
    
    ::sptool::SPFilterBase *_filter;
    ::sptool::SPAlgoBase *_algo;
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
