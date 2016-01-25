/**
 * \file MCClusterer.h
 *
 * \ingroup MCComp
 * 
 * \brief Class def header for a class MCClusterer
 *
 * @author david caratelli
 */

/** \addtogroup MCComp

    @{*/

#ifndef LARLITE_MCCLUSTERER_H
#define LARLITE_MCCLUSTERER_H

#include "Analysis/ana_base.h"
#include "MCBTAlg.h"

namespace larlite {
  /**
     \class MCClusterer
     User custom analysis class made by david caratelli
   */
  class MCClusterer : public ana_base{
  
  public:

    /// Default constructor
    MCClusterer();

    /// Default destructor
    virtual ~MCClusterer(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setClusterProducer(std::string s) { _cluster_producer = s; }
    
    void setMinEnergy(double e) { _mc_energy_min = e; }

  protected:

    // hit brack-tracking tool
    ::btutil::MCBTAlg _bt_algo;

    // cluster producer to be used
    std::string _cluster_producer;

    // minimum energy for a particle to be added to the map [MeV]
    double _mc_energy_min;
    

    
    
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
