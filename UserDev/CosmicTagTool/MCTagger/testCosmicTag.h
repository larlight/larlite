/**
 * \file testCosmicTag.h
 *
 * \ingroup MCTagger
 * 
 * \brief Class def header for a class testCosmicTag
 *
 * @author ah673
 */

/** \addtogroup MCTagger

    @{*/

#ifndef LARLITE_TESTCOSMICTAG_H
#define LARLITE_TESTCOSMICTAG_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class testCosmicTag
     User custom analysis class made by ah673
   */
  class testCosmicTag : public ana_base{
  
  public:

    /// Default constructor
    testCosmicTag(){ _name="testCosmicTag"; _fout=0;};

    /// Default destructor
    virtual ~testCosmicTag(){};

    /** IMPLEMENT in testCosmicTag.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in testCosmicTag.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in testCosmicTag.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

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
