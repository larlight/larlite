/**
 * \file cluster.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for container for larflow3dhits
 *
 * @author Taritree -- Oct 2018
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_LARFLOWCLUSTER_H
#define LARLITE_LARFLOWCLUSTER_H

#include "data_base.h"
#include "larflow3dhit.h"

namespace larlite{

  /**
     \class larflowcluster
     LArSoft Cluster class equivalent data container
  */
  class larflowcluster : public std::vector<larflow3dhit>, data_base {
  public:
    
    /// Default constructor
    larflowcluster() :
    data_base(data::kLArFlowCluster),
      isflashmatched(0),
      flash_tick(0),
      flash_time_us(0),
      flashmatch_score(0),
      matchedflash_producer(""),
      matchedflash_idx(0),
      has_truthmatch(0),
      is_neutrino(0),
      truthmatched_mctrackid(0),
      truthmatched_flashtick(0)
      { clear_data(); }

    void clear_data() {
      clear();
      _flash_data_v.clear();
      _flash_hypo_v.clear();
      _flash_truthmatched_v.clear();
    };
    
    /// Default destructor
    virtual ~larflowcluster(){}

    // reco flash matching
    int isflashmatched;     // found match-flash
    float flash_tick;       // relative to tick 0 of TPC image
    float flash_time_us;    // relative to trigger
    float flashmatch_score; // score quality from flash-match algo
    std::string matchedflash_producer; // producername for flash matched to cluster
    int         matchedflash_idx;      // index of matched flash in event_opflash vector
    /* std::vector<float>       alternative_match_score_v; */
    /* std::vector<int>         alternative_match_idx_v; */
    /* std::vector<std::string> alternative_match_producer_v; */
    
    // reco-truth matched info
    int has_truthmatch;
    int is_neutrino;
    int truthmatched_mctrackid;
    float truthmatched_flashtick;

    std::vector<float> _flash_data_v;
    std::vector<float> _flash_hypo_v;
    std::vector<float> _flash_truthmatched_v;

    
  };
  
  /**
     \class event_cluster
     A collection storage class of multiple clusters.
  */
  class event_larflowcluster : public std::vector<larlite::larflowcluster>,
    public event_base {
    
  public:
    
    /// Default constructor
    event_larflowcluster(std::string name="noname") : event_base(data::kLArFlowCluster,name) {clear_data();}
    
    /// Default copy constructor
    event_larflowcluster(const event_larflowcluster& original)
      : std::vector<larlite::larflowcluster>(original), event_base(original)
    {}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
    /// Default destructor
    ~event_larflowcluster(){}

  private:
    
  };
}
#endif
  
/** @} */ // end of doxygen group 
  
