/**
 * \file AlgoSingleE.h
 *
 * \ingroup ERTool
 * 
 * \brief This Algo returns a SPArticleSet of single electrons that are \n
          start-point isolated from other single electrons.
 *
 * @author kazuhiro + davidkaleko
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOSINGLEE_H
#define ERTOOL_ALGOSINGLEE_H

#include "Algo/AlgoEMPart.h"
#include "Algo/AlgoFindRelationship.h"
#include "Base/AlgoBase.h"

namespace ertool {

  /**
     \class AlgoSingleE
     User custom Algorithm class made by kazuhiro
   */
  class AlgoSingleE : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoSingleE();

    /// Default destructor
    virtual ~AlgoSingleE(){};

    /// Reset function
    virtual void Reset();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    virtual void ProcessEnd(TFile* fout);

    /// Override the ertool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to reconstruct the start-point isolated electrons
    virtual ParticleSet Reconstruct(const EventData &data);

    /// Set verbosity
    void setVerbose(bool on) { _verbose = on; _findRel.setDebug(on); }

  private:

    /// Function that takes in shower and decides if it is likely electron
    /// (it uses AlgoEMParticle)
    bool IsShowerElectron(const ertool::Shower* shower);

    /// Function to decide if two showers are correlated
    bool AreShowersStartPtCorrelated(const ertool::Shower* s1, const ertool::Shower* s2);

    /// Function to return a list of electron-like showers, given a list of showers
    const std::vector<const ertool::Shower*> ElectronLikeShowers(const std::vector< const ::ertool::Shower*>& showers);

    /// Function to return a list of start-point-isolated showers, given a list of showers
    const std::vector<const ertool::Shower*> IsolatedStartPtShowers(const std::vector<const ::ertool::Shower*>& showers);


  protected:

    // verbose flag
    bool _verbose;

    double _e_mass;

    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;

    //debug histos
    TH1F* _e_ll_values;
    TH1F* _dedx_values;
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
