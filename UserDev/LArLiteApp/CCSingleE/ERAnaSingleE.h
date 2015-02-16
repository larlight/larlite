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
#include "TH1D.h"

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

    /// Function to save efficiency vs. variable to histogram
    void MakeEffPlot(std::string varname,
		     int nbins, double xmin, double xmax);

  private:

    bool _debug;


    // Result tree comparison for reconstructed events
    TTree* _result_tree;

    //Number of reconstructed *particles* in the output PSet
    //(this is 1-to-1 with number of single electrons reconstructed)
    int _singleRECO;
    //Total number of MC electron/positron particles in the event
    int _MCelectrons;
    //Total number of MC gamma particles in the event
    int _MCgammas;
    //Number of reconstructed *showers* in the event data (could be e or g, etc)
    //Note: if running on MC, this is = # of MCShowers in the event 
    int _RECOshowers;
    //Number of reconstructed *tracks* in the event data
    int _RECOtracks;

    /// Energy of electron found to be SingleE, if only one exists in event
    /// otherwise, variable is set to 0
    double _E;    
    /// Whether this event was MID'd (if == 0: we found ==1 single electron)
    /// Note: this is 0 if _E is filled, otherwise this is 1 (redundant)
    int    _misID;
    double _Enu;   /// Neutrino energy
    int _Pdgnu;    /// Neutrino PDG
    double _Elep;  /// lepton from neutrino Energy
    int _Pdglep;   /// lepton from neutrino Pdg
    int _nProtons; /// number of protons ( E > 20 MeV)
    int _nNeutrons;/// number of neutrons (E > 20 MeV)
    int _nPiplus;  /// number of pi+ (E > 20 MeV)
    int _nPi0;     /// number of pi0 (E > 20 MeV)

    /// counters to cout to screen after running
    int _numEvts;
    int _singleE_ctr;

  };
}
#endif

/** @} */ // end of doxygen group 
