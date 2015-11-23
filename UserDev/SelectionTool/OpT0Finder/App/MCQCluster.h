/**
 * \file MCQCluster.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class MCQCluster
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/
#ifndef MCINTERACTION_H
#define MCINTERACTION_H

#include <iostream>
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "OpT0Finder/Base/BaseAlgorithm.h"

namespace flashana {
  /**
     \class MCQCluster
     User defined class MCQCluster ... these comments are used to generate
     doxygen documentation!
  */
  class MCQCluster : public flashana::BaseAlgorithm {
    
  public:
    
    /// Default constructor
    MCQCluster(const std::string name="MCQCluster");
    
    /// Default destructor
    ~MCQCluster(){}

    void Configure(const ::fcllite::PSet &pset);

    void Construct( const larlite::event_mctrack&,
                    const larlite::event_mcshower& );

    const std::vector<flashana::QCluster_t>& QClusters() const;

    const flashana::QCluster_t& QCluster(size_t) const;

    size_t MCTrack2QCluster(size_t) const;

    size_t MCShower2QCluster(size_t) const;

    const flashana::MCSource_t& MCObjectID(size_t) const;

    #ifndef __CINT__
    void Swap(std::vector<flashana::QCluster_t>&&,
	      std::vector<flashana::MCSource_t>&&);
    #endif
  private:
  
    flashana::MCSource_t Identify( const unsigned int,
				   const larlite::event_mctrack&,
				   const larlite::event_mcshower& ) const;
    
    double _light_yield;
    double _step_size;
    std::vector<flashana::QCluster_t> _qcluster_v;
    std::vector<size_t> _mctrack_2_qcluster;
    std::vector<size_t> _mcshower_2_qcluster;
    std::vector<flashana::MCSource_t> _qcluster_2_mcobject;

  };
}

#endif
/** @} */ // end of doxygen group 

