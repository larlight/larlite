/**
 * \file CosmicRateAna.h
 *
 * \ingroup Ana
 * 
 * \brief Class def header for a class CosmicRateAna
 *
 * @author kazuhiro
 */

/** \addtogroup Ana

    @{*/

#ifndef LARLITE_COSMICRATEANA_H
#define LARLITE_COSMICRATEANA_H

#include "Analysis/ana_base.h"
#include <TH1D.h>
namespace larlite {
  /**
     \class CosmicRateAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class CosmicRateAna : public ana_base{
  
  public:

    /// Default constructor
    CosmicRateAna()
    {
      _name="CosmicRateAna";
      _fout=0;
      _npe_cut_v.clear();
      _flash_ctr_v.clear();
      _total_window_ctr=0;

      size_t n_points=100;
      double spacing = 10.;

      for(size_t i=0; i<n_points; ++i) {
	_npe_cut_v.push_back(5*i);
	_flash_ctr_v.push_back(0);
      }
    }

    /// Default destructor
    virtual ~CosmicRateAna(){}

    /** IMPLEMENT in CosmicRateAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CosmicRateAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CosmicRateAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    std::vector<unsigned int> _npe_cut_v;
    std::vector<unsigned int> _flash_ctr_v;
    size_t _total_window_ctr;
    TH1D* _hBeamWindowWidth;
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
