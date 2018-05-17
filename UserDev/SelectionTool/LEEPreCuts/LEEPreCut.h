/**
 * \file LEEPreCut.h
 *
 * \ingroup LEEPreCuts
 * 
 * \brief Class def header for a class LEEPreCut
 *
 * @author twongjirad
 */

/** \addtogroup LEEPreCuts

    @{*/

#ifndef LARLITE_LEEPRECUT_H
#define LARLITE_LEEPRECUT_H

#include "Analysis/ana_base.h"
#include "FhiclLite/PSet.h"

#include "LEEPreCutAlgo.h"

namespace larlite {
  /**
     \class LEEPreCut
     User custom analysis class made by SHELL_USER_NAME
   */
  class LEEPreCut : public ana_base{
  
  public:

    /// Default constructor
    LEEPreCut(){ _name="LEEPreCut"; _fout=0;}

    /// Default destructor
    virtual ~LEEPreCut(){}

    virtual void configure( const fcllite::PSet& ps );
    
    /** IMPLEMENT in LEEPreCut.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in LEEPreCut.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in LEEPreCut.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void bindOutputVariablesToTree( TTree* ttree );

  protected:

    leeprecuts::LEEPreCutAlgo m_algo;
    std::string fOpHitProducer;
    int fBinTickWidth;
    int fWinStartTick;
    int fWinEndTick;
    int fVetoStartTick;
    int fVetoEndTick;    
    float fPEThreshold;    
    float fPMTMaxFrac;


    // result variables
    float _beamPE;
    float _vetoPE;
    float _maxFrac;
    int   _result;
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
