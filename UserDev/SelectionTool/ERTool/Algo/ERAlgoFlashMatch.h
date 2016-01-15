/**
 * \file ERAlgoFlashMatch.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoFlashMatch
 *
 * @author kazuhiro
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOFLASHMATCH_H
#define ERTOOL_ERALGOFLASHMATCH_H

#include "ERTool/Base/AlgoBase.h"
#include "OpT0Finder/Base/FlashMatchManager.h"
#include "OpT0Finder/Algorithms/LightPath.h"

namespace ertool {

  /**
     \class ERAlgoFlashMatch
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoFlashMatch : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoFlashMatch(const std::string& name="FlashMatch");

    /// Default destructor
    virtual ~ERAlgoFlashMatch(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    /// Getter function to return the FlashMatchManager
    ::flashana::FlashMatchManager& Manager() { return _mgr;}

  private:

    ::flashana::FlashMatchManager _mgr;

    double _beam_dt_min, _beam_dt_max;

  };
}
#endif

/** @} */ // end of doxygen group 
