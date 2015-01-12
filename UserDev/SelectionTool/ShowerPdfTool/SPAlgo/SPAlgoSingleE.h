/**
 * \file SPAlgoSingleE.h
 *
 * \ingroup SPAlgo
 * 
 * \brief This SPAlgo returns a SPArticleSet of single electrons that are \n
          start-point isolated from other single electrons.
 *
 * @author kazuhiro + davidkaleko
 */

/** \addtogroup SPAlgo

    @{*/

#ifndef SELECTIONTOOL_SPALGOSINGLEE_H
#define SELECTIONTOOL_SPALGOSINGLEE_H

#include "SPAlgo/SPAlgoEMPart.h"
#include "SPTBase/SPAlgoBase.h"

namespace sptool {

  /**
     \class SPAlgoSingleE
     User custom SPAFilter class made by kazuhiro
   */
  class SPAlgoSingleE : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoSingleE();

    /// Default destructor
    virtual ~SPAlgoSingleE(){};

    /// Reset function
    virtual void Reset();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    virtual void ProcessEnd(TFile* fout);

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to reconstruct the start-point isolated electrons
    virtual SPArticleSet Reconstruct(const SPAData &data);



  private:

    /// Function that takes in shower and decides if it is likely electron
    /// (it uses SPAlgoEMParticle)
    bool IsShowerElectron(const sptool::SPAShower shower);

    /// Function to decide if two showers are correlated
    bool AreShowersStartPtCorrelated(const sptool::SPAShower s1, const sptool::SPAShower s2);

    /// Function to return a list of electron-like showers, given a list of showers
    const std::vector<sptool::SPAShower> ElectronLikeShowers(const std::vector<sptool::SPAShower> showers);

    /// Function to return a list of start-point-isolated showers, given a list of showers
    const std::vector<sptool::SPAShower> IsolatedStartPtShowers(const std::vector<sptool::SPAShower> showers);

  protected:

    SPAlgoEMPart _alg_emp;


    //debug histos
    TH1F* e_ll_values;
    TH1F* dedx_values;
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
