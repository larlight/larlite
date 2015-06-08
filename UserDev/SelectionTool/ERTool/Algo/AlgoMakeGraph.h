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
     User custom SPAFilter class made by davidkaleko
   */
  class AlgoMakeGraph : public AlgoBase {
  
  public:

    /// Default constructor
    AlgoMakeGraph();

    /// Default destructor
    virtual ~AlgoMakeGraph(){};

    /// Reset function
    virtual void Finalize();

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// What to do once event-loop is over
    virtual void ProcessEnd(TFile* fout);

    /// Override the ertool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Set verbosity
    void setVerbose(bool on) { _verbose = on; _findRel.setDebug(on); _nodeMgr.setVerbose(on); }

    /// set cut on score (score = 1/IP)
    void setMinScore(double s) { _minScore = s; }
    

  private:

    /// Function to go from ertool RelationType_t to geotree RelationType_t
    ::geotree::RelationType_t GetGeoTreeRelation(RelationType_t rel);

    // verbose flag
    bool _verbose;

    // Algorithm to use to find correlations
    AlgoFindRelationship _findRel;

    // Manager for nodes
    ::geotree::Manager _nodeMgr;

    // minimum score for two object to be called correlated
    double _minScore;    

  };
}
#endif

/** @} */ // end of doxygen group 
