/**
 * \file ERAnaFindStart.h
 *
 * \ingroup Checker
 * 
 * \brief Class def header for a class ERAnaFindStart
 *
 * @author matthias
 */

/** \addtogroup Checker

    @{*/

#ifndef ERTOOL_ERANAFINDSTART_H
#define ERTOOL_ERANAFINDSTART_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"

namespace ertool {

  /**
     \class ERAnaFindStart
     User custom Analysis class made by kazuhiro
   */
  class ERAnaFindStart : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaFindStart(const std::string& name="ERAnaFindStart");

    /// Default destructor
    virtual ~ERAnaFindStart(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  private:
      double _shower_x;
      double _track_x;

      int _n_tracks = 0;

      TTree*_tracks;
      TTree*_showers;
      std::string _treename;

      // prepare TTree with variables
      void PrepareTreeVariables();

      void ResetTreeVariables();
  };
}
#endif

/** @} */ // end of doxygen group 
