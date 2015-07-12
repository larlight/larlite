/**
 * \file MC_CCQE_Filter.h
 *
 * \ingroup CCQESelection
 * 
 * \brief Class def header for a class MC_CCQE_Filter
 *
 * @author davidkaleko
 */

/** \addtogroup CCQESelection

    @{*/

#ifndef LARLITE_MC_CCQE_FILTER_H
#define LARLITE_MC_CCQE_FILTER_H

#include "Analysis/ana_base.h"
#include "TDatabasePDG.h"

namespace larlite {
  /**
     \class MC_CCQE_Filter
     User custom analysis class made by davidkaleko
   */
  class MC_CCQE_Filter : public ana_base{
  
  public:

    /// Default constructor
    MC_CCQE_Filter();

    /// Default destructor
    virtual ~MC_CCQE_Filter(){};

    /** IMPLEMENT in MC_CCQE_Filter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MC_CCQE_Filter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MC_CCQE_Filter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    // Exact number of mcshowers to look for
    void set_n_mcshowers(size_t kaleko){ _n_mcshowers_requested = kaleko; };

    // Minimum MCShower energy to consider (MEV)
    void set_min_mcshower_E(double kaleko){ _min_mcshower_E_requested = kaleko; };

    // Exact number of mctracks to look for
    void set_n_mctracks(size_t kaleko){ _n_mctracks_requested = kaleko; };

    // Minimum MCTrack kinetic energy to consider (MEV)
    void set_min_mctrack_E(double kaleko){ _min_mctrack_E_requested = kaleko; };

    protected:

    size_t total_events;
    size_t kept_events;

    /// Exact number of mcshowers to look for
    size_t _n_mcshowers_requested;
    /// Minimum MCShower energy to consider (MEV)
    double _min_mcshower_E_requested;
    /// Exact number of mctracks to look for
    size_t _n_mctracks_requested;
    /// Minimum MCTrack kinetic energy to consider (MEV)
    double _min_mctrack_E_requested;
    /// TDatabasePDG instance
    TDatabasePDG *_pdgdb;
    /// Particle mass map
    std::map<int,double> _massdb;
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
