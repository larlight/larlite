/**
 * \file MCShowerClusterer.h
 *
 * \ingroup ClusterRecoUtil
 * 
 * \brief This class takes in MCShower files, constructs fake hits (with 
 *  fixed width), clusters the hits together such that each MCShower has
 *  one cluster (in each view). EX: a single electron shower event will 
 *  have three clusters in its output, as there are 3 planes.
 *
 * @author davidkaleko
 */

/** \addtogroup ClusterRecoUtil

    @{*/

#ifndef RECOTOOL_MCSHOWERCLUSTERER_HH
#define RECOTOOL_MCSHOWERCLUSTERER_HH

#include "Analysis/ana_base.h"
#include "LArUtil/GeometryUtilities.h"

namespace larlite {
  /**
     \class MCShowerClusterer
     User custom analysis class made by davidkaleko
   */
  class MCShowerClusterer : public ana_base{
  
  public:

    /// Default constructor
    MCShowerClusterer(){ 
      _name="MCShowerClusterer"; 
      _fout=0;
      _group_overlapping_hits=true;
    }

    /// Default destructor
    virtual ~MCShowerClusterer(){}

    /** IMPLEMENT in MCShowerClusterer.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MCShowerClusterer.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MCShowerClusterer.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    

    //A function to check if a hit overlaps an already-made hit in time.
    //if it does not, function returns -1
    //if it does, function returns the index in original_hit_v of the hit it overlaps
    //note, i want this to return size_t but size_t can't be negative... how do i deal with this?
    int DoesHitOverlapExisting(event_hit* original_hit_v,UInt_t channel,Double_t start,Double_t end);
    
    //function to set whether to group overlapping hits as a single hit
    //(true by default.. reduces number of hits by a factor of ~10)
    void SetGroupOverlappingHits(bool flag) { _group_overlapping_hits = flag; };

  protected:

    size_t _nplanes;

    Double_t _hitwidth;

    bool _group_overlapping_hits;



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
