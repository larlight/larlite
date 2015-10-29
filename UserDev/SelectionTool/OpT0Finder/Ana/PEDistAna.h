/**
 * \file PEDistAna.h
 *
 * \ingroup Ana
 * 
 * \brief Class def header for a class PEDistAna
 *
 * @author kazuhiro
 */

/** \addtogroup Ana

    @{*/

#ifndef LARLITE_PEDISTANA_H
#define LARLITE_PEDISTANA_H

#include "Analysis/ana_base.h"
#include <TTree.h>
namespace larlite {
  /**
     \class PEDistAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class PEDistAna : public ana_base{
  
  public:

    /// Default constructor
    PEDistAna();

    /// Default destructor
    virtual ~PEDistAna(){}

    /** IMPLEMENT in PEDistAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in PEDistAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in PEDistAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetTimeRange(double min, double max)
    { _min_time = min; _max_time = max; }

    void SetProducers(std::string ophit   = "",
		      std::string opflash = "",
		      std::string opdigit = "",
		      std::string trigger = ""
		      )
    { _opdigit_producer = opdigit; _ophit_producer = ophit; _opflash_producer = opflash; _trigger_producer = trigger; }

  protected:

    TTree *_event_tree, *_run_tree;
    
    unsigned int _run, _subrun, _event;
    std::vector<double> _wf_event_pe, _hit_event_pe, _flash_event_pe;
    std::vector< std::vector<double> > _wf_tot_pe, _hit_tot_pe, _flash_tot_pe;
    std::string _opdigit_producer, _ophit_producer, _opflash_producer, _trigger_producer;
    double _min_time, _max_time;
    double _spe_area;
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
