/**
 * \file Hits.h
 *
 * \ingroup Investigator
 * 
 * \brief Class def header for a class Hits
 *
 * @author yuntse
 */

/** \addtogroup Investigator

    @{*/

#ifndef LARLITE_HITS_H
#define LARLITE_HITS_H

#include "Analysis/ana_base.h"

#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"

namespace larlite {
  /**
     \class Hits
     User custom analysis class made by SHELL_USER_NAME
   */
  class Hits : public ana_base{
  
  public:

    /// Default constructor
    Hits() { _name = "Hits"; fInputProducer = ""; _fout = 0; fEventTree = nullptr; }

    /// Default destructor
    virtual ~Hits(){}

    /** IMPLEMENT in Hits.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Hits.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Hits.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Function to set an input cluster/pfparticle producer name to work with
    void SetInputProducer( std::string name )
    { fInputProducer = name; }

  protected:

    std::string fInputProducer;

    /// Analysis TTree. Filled once per event.
    TTree *fEventTree;

    int _event, _subrun, _run;

    struct EventTreeParams_t {

      int run;
      int subrun;
      int event;
      int nhits[3];
      double charges[3];

    } fEventTreeParams;

    /// Function to prepare TTrees
    void InitializeAnaTrees();

    void ResetEventTreeParams();

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
