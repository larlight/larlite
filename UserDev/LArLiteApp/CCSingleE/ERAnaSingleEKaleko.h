/**
 * \file ERAnaSingleEKaleko.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaSingleEKaleko
 *
 * @author david
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANASINGLEEKALEKO_H
#define ERTOOL_ERANASINGLEEKALEKO_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "GeoAlgo/GeoAlgo.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "DataFormat/mctruth.h"

namespace ertool {

  /**
     \class ERAnaSingleEKaleko
     User custom Analysis class made by kazuhiro
   */
  class ERAnaSingleEKaleko : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaSingleEKaleko();

    /// Default destructor
    virtual ~ERAnaSingleEKaleko(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    void ProcessEnd(TFile* fout);

    /// Function to re-set TTree variables
    void ResetTreeVariables();

    /// Function to save efficiency vs. variable to histogram
    void MakeEffPlot(std::string varname,
		     int nbins, double xmin, double xmax);

    /// get reported efficiency
    double getEfficiency() { return _eff; }

    /// set the energy cut to be used when counting particles
    void SetECut(double c) { _eCut = c; }

  private:

    // energy cut for counting particles
    float _eCut;

    // Result tree comparison for reconstructed events
    TTree* _result_tree; 

    //Number of reconstructed *particles* in the output PSet
    //(this is 1-to-1 with number of single electrons reconstructed)
    int _n_singleReco;
    int _n_protonsInt;     /// Number of proton mc *tracks* from interaction
    int _n_protonsIntReco; /// Number of reconstructed proton *tracks* from interaction in the return ParticleSet
    int    _misID;     /// Whether this event was MID'd (if == 0: we found == 1 single electron)
    double _e_protonsInt, _e_protonsIntReco; /// Energy of proton tracks @ neutrino vertex
    double _e_nu, _e_nuReco;     /// Neutrino energy
    int _pdg_nu;       /// Neutrino PDG
    double _e_lep;     /// lepton from neutrino Energy
    int _pdg_lep;      /// lepton from neutrino Pdg
    double _e_lepReco; /// lepton E from reconstruction (i.e. SingleE energy if found)
    // value of efficiency measured at the end
    double _eff;
    bool _inFidVol; /// If true neutrino vertex is inside of the fiducial volume (uhh... 25cm from each wall?)

    //Counters necessary to compute efficiency
    size_t _single_E_ctr;
    size_t _numEvts;

    //truth neutrino vertex necessary to count MC tracks near vtx
    double _x_nu, _y_nu, _z_nu;

    ::geoalgo::GeoAlgo _geoAlgo;
    ::geoalgo::AABox fTPC;
    AlgoFindRelationship _findRel;

    TH2F* _h_e_nu_correlation;
   
  };
}
#endif

/** @} */ // end of doxygen group 
