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
  larflowcluster() : data_base(data::kLArFlowCluster) { clear_data(); }

    void clear_data() { clear(); };
    
    /// Default destructor
    virtual ~larflowcluster(){}
    
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
  
