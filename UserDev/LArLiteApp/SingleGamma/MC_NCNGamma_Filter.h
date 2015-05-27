/**
 * \file MC_NCNGamma_Filter.h
 *
 * \ingroup SingleGamma
 * 
 * \brief Class def header for a class MC_NCNGamma_Filter
 *
 * @author Joseph Zennamo, jzennamo@uchicago.edu
 */

/** \addtogroup SingleGamma

    @{*/

#ifndef LARLITE_MC_NCNGAMMA_FILTER_H
#define LARLITE_MC_NCNGAMMA_FILTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"

namespace larlite {
  /**
     \class MC_NCNGamma_Filter
     User custom analysis class made by SHELL_USER_NAME
   */
  class MC_NCNGamma_Filter : public ana_base{
  
  public:

    int max_events;
    size_t looped_events;

    /// Default constructor
    MC_NCNGamma_Filter(){ _name="MC_NCNGamma_Filter"; _fout=0;}

    /// Default destructor
    virtual ~MC_NCNGamma_Filter(){}

    virtual bool initialize();
    virtual bool analyze(storage_manager* storage);

    void setMaxNEvents(int N) {max_events = N;}
    void flip(bool on) {_flip = on;}

    virtual bool finalize();

  protected:
    size_t total_events;
    size_t kept_events;
    bool _flip;


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
