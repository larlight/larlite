/**
 * \file ERAlgoNueSharedFlashMerger.h
 *
 * \ingroup Algo
 *
 * \brief Class that looks for a reconstructed nue, loops over other tracks in the event,
 *        and if they are associated with the same flash, and they are "nearby" (configurable),
 *        they get added as a child of the nue.
 *        Also, if the nue has an electron daughter, and a shower is found that backward-projects
 *        to almost-the-same-place as the electron backward-projected, it sets the nue process string
 *        as kPiZeroMID (later, perhaps this should delete the neutrino, but particle deletion DNE yet)
 *        Note this code does nothing if "reuse_flash" parameter is not toggled true!
 *
 * @author davidkaleko
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGONUESHAREDFLASHMERGER_H
#define ERTOOL_ERALGONUESHAREDFLASHMERGER_H

#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"

namespace ertool {

  /**
     \class ERAlgoNueSharedFlashMerger
     User custom Algorithm class made by kaleko
   */
  class ERAlgoNueSharedFlashMerger : public AlgoBase {

  public:

    /// Default constructor
    ERAlgoNueSharedFlashMerger(const std::string& name = "ERAlgoNueSharedFlashMerger");

    /// Default destructor
    virtual ~ERAlgoNueSharedFlashMerger() {};

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

    void setDebug(bool flag) { _debug = flag; }

    /// Function to check wether a shower is e- or gamma-like
    /// Returns true if gamma-like
    bool isGammaLike(const double dedx, double radlen,bool forceRadLen=false);


  protected:

    AlgoFindRelationship _findRel;
    AlgoEMPart _alg_emp;

  private:

    bool _debug;

    ::geoalgo::GeoAlgo _geoalgo;

  };
}
#endif

/** @} */ // end of doxygen group
