/**
 * \file ERAlgoTrackID.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoTrackID
 *
 * @author rmurrell
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOTRACKID_H
#define ERTOOL_ERALGOTRACKID_H

#include "ERTool/Base/AlgoBase.h"

#include "TGraph.h"
#include "TH1D.h"

#include "GeoAlgo/GeoAABox.h"

namespace ertool {

  /**
     \class ERAlgoTrackID
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoTrackID : public AlgoBase {

    TH1D * contained_chi2;
    TH1D * uncontained_chi2;

    geoalgo::AABox volume;
  
  public:

    /// Default constructor
    ERAlgoTrackID(const std::string& name="ERAlgoTrackID");

    /// Default destructor
    virtual ~ERAlgoTrackID(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    Double_t Chi2Test(TGraph * g);

    void FitTrack(Track const & t, Particle & p);

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
