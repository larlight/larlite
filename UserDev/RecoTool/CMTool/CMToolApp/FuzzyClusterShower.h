/**
 * \file FuzzyClusterShower.h
 *
 * \ingroup CMToolApp
 * 
 * \brief Class def header for a class FuzzyClusterShower
 *
 * @author USER_NAME
 */

/** \addtogroup CMToolApp

    @{*/

#ifndef FUZZYCLUSTERSHOWER_H
#define FUZZYCLUSTERSHOWER_H

#include "ClusterMergerArray.h"
#include "CMTool/CMTAlgPriority/CPAlgoIgnoreTracks.h"
#include "CMTool/CMTAlgMerge/CBAlgoShortestDist.h"
#include "CMTool/CMTAlgMerge/CBAlgoStartTrack.h"
#include "CMTool/CMTAlgMerge/CBAlgoPolyContain.h"
#include "CMTool/CMTAlgMerge/CBAlgoCenterOfMass.h"
#include "CMTool/CMTAlgMerge/CBAlgoPolyOverlap.h"
#include "CMTool/CMTAlgMerge/CBAlgoPolyShortestDist.h"
#include "CMTool/CMTAlgMerge/CBAlgoTrackSeparate.h"
#include "CMTool/CMTAlgMerge/CBAlgoOutOfConeSeparate.h"
#include "CMTool/CMTAlgMerge/CBAlgoAngleIncompat.h"
#include "CMTool/CMTAlgMerge/CBAlgoArray.h"
namespace larlite {
  /**
     \class FuzzyClusterShower
     User custom analysis class made by kazuhiro
   */
  class FuzzyClusterShower : public ClusterMergerArray {
  
  public:

    /// Default constructor
    FuzzyClusterShower();

    /// Default destructor
    virtual ~FuzzyClusterShower(){}

    /** IMPLEMENT in FuzzyClusterShower.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in FuzzyClusterShower.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in FuzzyClusterShower.cc! 
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
