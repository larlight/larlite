/**
 * \file AlgoMakeGraph.h
 *
 * \ingroup ERTool
 * 
 * \brief Class def header for a class AlgoMakeGraph
 *
 * @author davidc1
 */

/** \addtogroup ERTool

    @{*/

#ifndef ERTOOL_ALGOMAKEGRAPH_H
#define ERTOOL_ALGOMAKEGRAPH_H

#include "ERTool/Algo/AlgoFindRelationship.h"
#include "TreeGraph/Manager.h"
#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class AlgoMakeGraph
   */
  class AlgoMakeGraph : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoMakeGraph(const std::string &name="MakeGraph");

    /// Default destructor
    ~AlgoMakeGraph(){};

    /// Reset function
    void Reset(){}
    
    void Finalize();

    /// What to do before event-loop begins
    void ProcessBegin();

    /// What to do once event-loop is over
    void ProcessEnd(TFile* fout);

    /// Configuration setter
    void AcceptPSet(const ::fcllite::PSet& cfg);
    
    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Set verbosity
    void setVerbose(bool on) { _verbose = on; _findRel.setDebug(on); _nodeMgr->setVerbose(on); }

    /// set cut on score (score = 1/IP)
    void setMinScore(double s) { _minScore = s; }

    /// Set Node Manager
    void setManager(::geotree::Manager* mgr) { _nodeMgr = mgr; }
    

  private:

    /// Function to go from ertool RelationType_t to geotree RelationType_t
    ::geotree::RelationType_t GetGeoTreeRelation(RelationType_t rel);

    // verbose flag
    bool _verbose;

    // Algorithm to use to find correlations
    AlgoFindRelationship _findRel;

    // Manager for nodes
    ::geotree::Manager* _nodeMgr;

    // minimum score for two object to be called correlated
    double _minScore;    

  };
}
#endif

/** @} */ // end of doxygen group 
