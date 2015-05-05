/**
 * \file Pi0ContainmentFilter.h
 *
 * \ingroup CCQESelection
 * 
 * \brief Class def header for a class Pi0ContainmentFilter
 *
 * @author david
 */

/** \addtogroup CCQESelection

    @{*/

#ifndef LARLITE_PI0CONTAINMENTFILTER_H
#define LARLITE_PI0CONTAINMENTFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class Pi0ContainmentFilter
     User custom analysis class made by david
   */
  class Pi0ContainmentFilter : public ana_base{
  
  public:

    /// Default constructor
    Pi0ContainmentFilter(){ _name="Pi0ContainmentFilter"; _fout=0; _EC = 0.75; _Energy= 20;};

    /// Default destructor
    virtual ~Pi0ContainmentFilter(){};

    /** IMPLEMENT in Pi0ContainmentFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Pi0ContainmentFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Pi0ContainmentFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetEC(double EC) { _EC = EC ; }

    void SetEnergyCut(double EnergyCut) { _Energy = EnergyCut; }

    protected:

    int _kept_events;
    int _total_events;

    int _n_pi0;
    int _n_E;
    int _n_detProf ;

    double _EC ;
    double _Energy ;

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
