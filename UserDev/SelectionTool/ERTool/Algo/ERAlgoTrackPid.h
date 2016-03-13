/**
 * \file ERAlgoTrackPid.h
 *
 * \ingroup Mode13
 * 
 * \brief Class def header for a class ERAlgoTrackPid
 *
 * @author elenag
 */

/** \addtogroup Mode13

    @{*/

#ifndef ERTOOL_ERALGOTRACKPID_H
#define ERTOOL_ERALGOTRACKPID_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoSphere.h"
#include <algorithm> // for std::find
#include <utility>
#include <TTree.h>

namespace ertool {

  /**
     \class ERAlgoTrackPid
     User custom Algorithm class made by kazuhiro
  */
  class ERAlgoTrackPid : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoTrackPid(const std::string& name="ERAlgoTrackPid");

    /// Default destructor
    virtual ~ERAlgoTrackPid(){};

    /// Reset function
    void Reset();

    void Finalize();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void InitializeHistos();
   
    /*
    /// Function to set minumum required length for a muon track
    void setMinMuonLength(double l) { _minMuonLength = l; }
    
    /// Function to set the maximum allowed muon-michel separation
    void setMaxDistance(double d) { _maxDistance = d; }
    */
    
  private:
    /// clear tree
    void ClearTree();

    
  protected:

    // histograms to be filled in Algo

    /*
    TH1F* shower_st_to_track_end;
    
    // cut values that are settable by user
    double _minMuonLength;
    double _maxDistance;
    */

    TTree* _algoPid_tree ;
    int _part_pid         ;
    

    
  };
}
#endif

/** @} */ // end of doxygen group 

