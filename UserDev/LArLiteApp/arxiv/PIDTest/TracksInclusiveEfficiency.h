/**
 * \file TracksInclusiveEfficiency.h
 *
 * \ingroup PIDTest
 * 
 * \brief Class def header for a class TracksInclusiveEfficiency
 *
 * @author elenag
 */

/** \addtogroup PIDTest

    @{*/

#ifndef LARLITE_TRACKSINCLUSIVEEFFICIENCY_H
#define LARLITE_TRACKSINCLUSIVEEFFICIENCY_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class TracksInclusiveEfficiency
     User custom analysis class made by SHELL_USER_NAME
   */
  class TracksInclusiveEfficiency : public ana_base{
  
  public:

    /// Default constructor
    TracksInclusiveEfficiency(){ _name="TracksInclusiveEfficiency"; _fout=0;}

    /// Default destructor
    virtual ~TracksInclusiveEfficiency(){}

    /** IMPLEMENT in TracksInclusiveEfficiency.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TracksInclusiveEfficiency.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TracksInclusiveEfficiency.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetTrackProducer(const std::string name)
    { fTrackProducer = name; }

  protected:
    double efficiency = 1.;
    double purity = 1.;
    int numbMCTrackEff   = 0;
    int numbMCTrackPur   = 0;
    int numbRecoTrackEff = 0;
    int numbRecoTrackPur = 0;
    std::string fTrackProducer;
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
