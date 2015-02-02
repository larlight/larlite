/**
 * \file AlgoSingleE.h
 *
 * \ingroup ERTool
 * 
 * \brief This Algo returns a SPArticleSet of single electrons that are \n
          start-point isolated from other single electrons.
 *
 * @author kazuhiro + davidkaleko + davidc
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOSINGLEE_H
#define ERTOOL_ALGOSINGLEE_H

#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"

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

    void setVtxToTrkStartDist(double d) { _vtxToTrkStartDist = d; }
    void setVtxToTrkDist(double d) { _vtxToTrkDist = d; }
    void setVtxToShrStartDist(double d) { _vtxToShrStartDist = d; }
    void setMaxIP(double d) { _maxIP = d; }

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
    // electron mass
    double _e_mass;

    // constants to be used for decision-making
    // minimum distance that a reco-vertex must be away from the
    // start of a track for the vtx to be considered "on the track"
    // and the shower to come from the track, instead of the shower
    // and track being siblings.
    double _vtxToTrkStartDist;
    // maximum distance from the entire track for the reco-vertex
    // for the vertex to be considered on the track and the shower
    // to be considered as coming from the track
    double _vtxToTrkDist;
    // Max distance from shower start that the vertex can be
    // this value should be a few radiation lengths: the cut 
    // is placed to remove cases where the shower is meters
    // away from the vertex. Probably an accidental correlation
    double _vtxToShrStartDist;
    // Maximum impact parameter allowed between the two objects
    // if larger it means that the two lines/segments do not come
    // within this value at any point and they therefore are 
    // assumed to not be correlated
    double _maxIP;

    // Other algorithms to use
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;
    // GeoAlgo Tool
    ::geoalgo::GeoAlgo _geoAlgo;

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
