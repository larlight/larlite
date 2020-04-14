/**
 * \file CRTVeto.h
 *
 * \ingroup LEEPreCuts
 * 
 * \brief Class def header for a class CRTVeto
 *
 * @author rshara01
 */

/** \addtogroup LEEPreCuts

    @{*/

#ifndef LARLITE_CRTVETO_H
#define LARLITE_CRTVETO_H

#include "Analysis/ana_base.h"
#include "DataFormat/crthit.h"
#include "FhiclLite/PSet.h"

//#include "algosrc/LEEPreCutAlgo.h"

namespace larlite {
  /**
     \class CRTVeto
     User custom analysis class made by SHELL_USER_NAME
   */
  class CRTVeto : public ana_base{
  
  public:

    typedef enum { kBNB=0, kMC, kEXTBNB, kOVERLAY } FileType_t;

    /// Default constructor
    CRTVeto() : _tree(nullptr) { _name="CRTVeto"; _fout=0; }

    /// Default destructor
    virtual ~CRTVeto(){}

    virtual void configure( const fcllite::PSet& ps );
    
    /** IMPLEMENT in CRTVeto.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in CRTVeto.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in CRTVeto.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /** Apply precuts */
    bool apply( const larlite::event_opflash& opflash_v, const larlite::event_crthit& crthit_v );

    /** Set default parameters */
    void setDefaults( larlite::CRTVeto::FileType_t ft=kBNB );

    /** Set input producers */
    void setOpFlashProducer( std::string name ) { fOpFlashProducer=name; };
    void setCRTHitProducer( std::string name )  { fCRTHitProducer=name; };

  protected:
    TTree* _tree;

  public:
    void bindOutputVariablesToTree( TTree* ttree );
    
  protected:

    //leeprecuts::LEEPreCutAlgo m_algo;
    std::string fOpFlashProducer;
    std::string fCRTHitProducer;
    float fWinStart;
    float fWinEnd;

    float fDeltaT;
    float fCRTHitPE;
    int _run;
    int _subrun;
    int _event;

    int   _passed;
    std::vector<float> _opflashT;
    std::vector<float> _crthitT;
    std::vector<float> _crthitPE;
    std::vector<float> _DT;
    
  public:
    
    int   passes()    { return _passed; };
    std::vector<float> opflashT() { return _opflashT; };
    std::vector<float> crthitT() { return _crthitT; };
    std::vector<float> crthitPE() { return _crthitPE; };
    std::vector<float> DT() { return _DT; };
    
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
