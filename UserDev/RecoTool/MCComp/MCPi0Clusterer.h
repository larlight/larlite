/**
 * \file MCPi0Clusterer.h
 *
 * \ingroup MCComp
 * 
 * \brief Class def header for a class MCPi0Clusterer
 *
 * @author david caratelli
 */

/** \addtogroup MCComp

    @{*/

#ifndef LARLITE_MCPi0CLUSTERER_H
#define LARLITE_MCPi0CLUSTERER_H

#include "Analysis/ana_base.h"
#include "MCBTAlg.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/SpaceChargeMicroBooNE.h"

namespace larlite {
  /**
     \class MCPi0Clusterer
     User custom analysis class made by david caratelli
   */
  class MCPi0Clusterer : public ana_base{
  
  public:

    /// Default constructor
    MCPi0Clusterer();

    /// Default destructor
    virtual ~MCPi0Clusterer(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();
    
    void setMinEnergy(double e) { _mc_energy_min = e; }

  protected:

    // hit brack-tracking tool
    ::btutil::MCBTAlg _bt_algo;

    // minimum energy for a particle to be added to the map [MeV]
    double _mc_energy_min;
    int _event ;    

    larutil::SpaceChargeMicroBooNE *_SCE;

    double _offset;
    double _time2cm;

    
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
