/**
 * \file ERAnaSingleE.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaSingleE
 *
 * @author david
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANASINGLEE_H
#define ERTOOL_ERANASINGLEE_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"

#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaSingleE
     User custom Analysis class made by kazuhiro
   */
  class ERAnaSingleE : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaSingleE();

    /// Default destructor
    virtual ~ERAnaSingleE(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    void SetDebug(bool on) { _debug = on; }

    void ProcessEnd(TFile* fout);

  private:

    bool _debug;

    // Analysis tree with MC level info comparison
    // between EventData and ParticleSet
    TTree* _ana_tree;
    int _singleRECO, _singleMC;
    int _RECOshowers, _RECOtracks;
    int _MCelectrons, _MCgammas;

    // Result tree comparison for reconstructed events
    TTree* _result_tree;
    double _E;     /// Energy of electron shower found to be SingleE
    int    _misID; /// if ==0 -> we found a SingleE when there is one
    double _Enu;   /// Neutrino energy
    int _Pdgnu;    /// Neutrino PDG
    double _Elep;  /// lepton from neutrino Energy
    int _Pdglep;   /// lepton from neutrino Pdg

    int _numEvts;
    int _singleE;

  };
}
#endif

/** @} */ // end of doxygen group 
