/**
 * \file MCShowerMatchAlg.h
 *
 * \ingroup MCShowerBT
 * 
 * \brief Class def header for a class MCShowerMatchAlg
 *
 * @author kazuhiro
 */

/** \addtogroup MCShowerBT

    @{*/
#ifndef RECOTOOL_MCSHOWERMATCHALG_H
#define RECOTOOL_MCSHOWERMATCHALG_H

#include <iostream>
#include "Base/larlite_base.h"
#include "LArUtil/Geometry.h"
#include "DataFormat/storage_manager.h"
#include "MCShowerBTAlg.h"
#include "MCShowerBTException.h"
namespace larlite {
  /**
     \class MCShowerMatchAlg
     User defined class MCShowerMatchAlg ... these comments are used to generate
     doxygen documentation!
  */
  class MCShowerMatchAlg : public larlite_base{
    
  public:
    
    /// Default constructor
    MCShowerMatchAlg();
    
    /// Default destructor
    virtual ~MCShowerMatchAlg(){};
    
    /// Constructs needed information for Shower=>MCShower matching
    bool BuildMap(storage_manager* storage,
		  const std::string cluster_producer);

    /**
       For a specified MCShower (via index in event_mcshower), returns best-matched clusters 
       represented by an index number of event_cluster. The return vector length is same as
       # of planes, and "-1" if there is no cluster found on the corresponding plane.
    */
    const std::vector<int>& BestClusters(const size_t mcshower_index) const;

    /**
       For a specified cluster (via event_cluster index), compute the correctness for the specified
       mcshower (via event_mcshower index).
     */
    double ClusterCorrectness(const size_t cluster_index,
			      const size_t mcshower_index) const;

    /**
       Similar to ClusterCorrectness() function except this one computes the correctness for multiple
       clusters. It is meant to compute the correctness for a cluster combination that is matched 
       across plane to represent one 3D shower object. The return is a pair of best-matched MCShower
       (in terms of event_mcshower index number) and correctness.
     */
    std::pair<size_t,double> ShowerCorrectness(const std::vector<unsigned int> cluster_indices) const;


    /**
       For a specified cluster (via event_cluster index number), compute cluster efficiency and
       purity in terms of specified mcshower (via event_mcshower index number)
     */
    std::pair<double,double>  ClusterEP(const size_t cluster_index,
					const size_t mcshower_index) const;

    /**
       For a specified mcshower (via event_mcshower index number), compute the best-matched cluster's
       efficiency and purity from the specified plane ID. Internally this function calls BestClusters()
       to find the best cluster on each plane, then calls ClusterEP() to retrieve the efficiency and
       purity.
     */
    std::pair<double,double> BestClusterEP(const size_t mcshower_index,
					   const size_t plane_id) const;

    /// BTAlgo getter
    const MCShowerBTAlg& BTAlg() const { return fBTAlgo; }

    /// Set maximum energy for MCShowers to be considered
    void SetMaxEnergyCut(const double energy) { fBTAlgo.SetMaxEnergyCut(energy); }

    /// Set minimum energy for MCShowers to be considered
    void SetMinEnergyCut(const double energy) { fBTAlgo.SetMinEnergyCut(energy); }

  protected:

    // Internal function to convert mcshower_index (absolute index in event_mcshower) to an internally-used index
    size_t __IndexConversion__(const size_t mcshower_index) const;
  
    /// MCShowerBTAlg instance 
    MCShowerBTAlg fBTAlgo;

    std::string fClusterProducer;

    std::vector<int> _mcshower_index_v;

    std::vector<size_t> _view_to_plane;

    std::vector<std::vector<double> > _summed_mcq;
    std::vector<std::vector<double> > _cluster_mcq_v;

    std::vector<unsigned char> _cluster_plane_id;
    std::vector<std::vector<int> > _bmatch_id;

  };
}
#endif
/** @} */ // end of doxygen group 

