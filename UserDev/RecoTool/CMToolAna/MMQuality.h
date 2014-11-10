/**
 * \file MMQuality.h
 *
 * \ingroup CMToolAna
 * 
 * \brief Class def header for a class MMQuality
 *
 * @author kazuhiro
 */

/** \addtogroup CMToolAna

    @{*/

#ifndef LARLITE_MMQUALITY_H
#define LARLITE_MMQUALITY_H

#include <TH1D.h>
#include <TH2D.h>
#include <map>
#include "Analysis/ana_base.h"
#include "MCShowerBT/MCShowerMatchAlg.h"
#include "CMToolAnaException.h"

namespace larlite {
  /**
     \class MMQuality
     User custom analysis class made by kazuhiro
   */
  class MMQuality : public ana_base{
  
  public:

    /// Default constructor
    MMQuality();

    /// Default destructor
    virtual ~MMQuality(){};

    /**
       Setter function for a shower producer name.
       Note you can only specify either shower producer or cluster producer.
       If shower producer is set, cluster producer's name is looked up through
       association.
    */
    void SetShowerProducer(const std::string name)
    { fShowerProducer = name; }

    /**
       Setter function for a cluster producer name.
       Note you can only specify either shower producer or cluster producer.
       If shower producer is set, cluster producer's name is looked up through
       association.
    */
    void SetClusterProducer(const std::string name)
    { fClusterProducer = name; }

    /// Set maximum energy for MCShowers to be considered
    void SetMaxEnergyCut(const double energy) { fBTAlg.SetMaxEnergyCut(energy); }

    /// Set minimum energy for MCShowers to be considered
    void SetMinEnergyCut(const double energy) { fBTAlg.SetMinEnergyCut(energy); }

    /** IMPLEMENT in MMQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MMQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MMQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    /// Shower back tracking algorithm
    MCShowerMatchAlg fBTAlg;

    /// Shower Producer's Name
    std::string fShowerProducer;

    /// Cluster Producer's Name
    std::string fClusterProducer;

    /// Cluster efficiency (entry per MCShower)
    std::vector<TH1D*> vShowerClusterEff;

    /// Cluster purity (entry per MCShower)
    std::vector<TH1D*> vShowerClusterPur;

    /// Matching correctness
    TH1D *hMatchCorrectness;

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
