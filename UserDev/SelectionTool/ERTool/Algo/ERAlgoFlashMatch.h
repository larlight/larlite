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
    ERAlgoFlashMatch(const std::string& name = "FlashMatch");

    /// Default destructor
    virtual ~ERAlgoFlashMatch() {};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout = nullptr);

    /// Getter function to return the FlashMatchManager
    ::flashana::FlashMatchManager& Manager() { return _mgr;}

    /// Setter function to ignore shower particles (useful for validation scripts)
    void SetIgnoreShowers(bool flag) { _ignore_showers = flag; }

    /// Setter function to ignore cosmic particles
    void SetIgnoreCosmics(bool flag) { _ignore_cosmics = flag; }

  private:

    ::flashana::FlashMatchManager _mgr;

    ::flashana::LightPath LP;
    double _light_yield; // Photons per MEV, gotten from LightPath configured instance

    double _beam_dt_min, _beam_dt_max;
    bool _ignore_showers; // Don't look for a flash for an intxn with base particle that is kShower
    bool _ignore_cosmics; // Don't look for a flash for an intxn with base particle that is kCosmic
    
    TTree * _match_tree;
    double _mct;
    double _mct_x;
    double _ft;
    double _e;


  };
}
#endif

/** @} */ // end of doxygen group
