/**
 * \file ERAlgoToy.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoToy
 *
 * @author kazuhiro
 */

/** \addtogroup Algo

    @{*/

#ifndef SELECTIONTOOL_ERTOOL_ERALGOTOY_H
#define SELECTIONTOOL_ERTOOL_ERALGOTOY_H

#include "ERTool/Base/AlgoBase.h"
#include <TH1D.h>
namespace ertool {

  /**
     \class ERAlgoToy
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoToy : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoToy(const std::string& name="ERAlgoToy");

    /// Default destructor
    virtual ~ERAlgoToy(){};

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

  private:

    /// Minimum radius to claim a vtx point
    double _vtx_min_radius;

    /// Radius histogram
    TH1D* _hRadius;

  };
}
#endif

/** @} */ // end of doxygen group 
