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
#include <TTree.h>
#include <map>
#include "Analysis/ana_base.h"
#include "MCComp/MCMatchAlg.h"
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
       Note you can only specify only one of the three possible (shower,
       cluster,pfparticle) producer names. Otherwise it is ambiguous what
       type of data-product the efficiency should be calculated for.
       If showers or pfparticles are used, the cluster producer name
       is looked up through associations
    */
    void SetShowerProducer(const std::string name)
    { fShowerProducer = name; }

    /**
       Setter function for a PFParticle producer name.
       Note you can only specify only one of the three possible (shower,
       cluster,pfparticle) producer names. Otherwise it is ambiguous what
       type of data-product the efficiency should be calculated for.
       If showers or pfparticles are used, the cluster producer name
       is looked up through associations
    */
    void SetPFParticleProducer(const std::string name)
    { fPFParticleProducer = name; }

    /**
       Setter function for a cluster producer name.
       Note you can only specify only one of the three possible (shower,
       cluster,pfparticle) producer names. Otherwise it is ambiguous what
       type of data-product the efficiency should be calculated for.
       If showers or pfparticles are used, the cluster producer name
       is looked up through associations
    */
    void SetClusterProducer(const std::string name)
    { fClusterProducer = name; }

    /// Set maximum energy for MCShowers to be considered
    void SetMaxEnergyCut(const double energy) { _mc_energy_min = energy; }

    /// Set minimum energy for MCShowers to be considered
    void SetMinEnergyCut(const double energy) { _mc_energy_max = energy; }

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

    int _mcshowerTot ;
    int _recoClusterTot ;

    std::vector<int> fMCShower;
    std::vector<int> fRecoHit;
	        
    std::vector<double> fRecoEff_0;
    std::vector<double> fRecoEff_1;
    std::vector<double> fRecoEff_2;
	              
    std::vector<double> fRecoPur_0;
    std::vector<double> fRecoPur_1;
    std::vector<double> fRecoPur_2;
	              
    std::vector<double> fBestEff_0;
    std::vector<double> fBestEff_1;
    std::vector<double> fBestEff_2;
	              
    std::vector<double> fBestPur_0;
    std::vector<double> fBestPur_1;
    std::vector<double> fBestPur_2;


    /// minimum energy cut to find a relevant MC Shower
    double _mc_energy_min;

    /// maximum energy cut to find a relevant MC Shower
    double _mc_energy_max;

    /// Shower back tracking algorithm
    ::btutil::MCMatchAlg fBTAlg;

    /// Shower Producer's Name
    std::string fShowerProducer;

    /// Cluster Producer's Name
    std::string fClusterProducer;

    /// PFParticle Producer's Name
    std::string fPFParticleProducer;

    /// Cluster efficiency (entry per MCShower)
    //  std::vector<TH1D*> vShowerClusterEff;

    /// Cluster purity (entry per MCShower)
    //std::vector<TH1D*> vShowerClusterPur;

    /// Cluster efficieny vs  purity (entry per MCShower)
    //std::vector<TH2D*> vShowerClusterEffVsPur;

    /// Cluster efficiency (entry per RecoCluster)

    //std::vector<TH1D*> vRecoShowerClusterEff;

    /// Cluster purity (entry per RecoCluster)

    //std::vector<TH1D*> vRecoShowerClusterPur;

    /// Cluster efficieny vs  purity (entry per RecoCluster)

    //std::vector<TH2D*> vRecoShowerClusterEffVsPur;

    /// Matching correctness
    TH1D *hMatchCorrectness;

    /// Number of hits
    //TH1D *hClusterHits;

    // My analysis TTree
    TTree *fMMQualityTree;

    /// Function to prepare TTree
    void InitializeAnaTree();

    /// Function to clear TTree
    void ClearTreeVar();
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
