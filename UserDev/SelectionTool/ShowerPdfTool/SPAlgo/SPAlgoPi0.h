/**
 * \file SPAlgoPi0.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPAlgoPi0
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef SELECTIONTOOL_SPALGOPI0_H
#define SELECTIONTOOL_SPALGOPI0_H

#include "SPAlgo/SPAlgoEMPart.h"

namespace sptool {

  /**
     \class SPAlgoPi0
     User custom SPAFilter class made by kazuhiro
   */
  class SPAlgoPi0 : public SPAlgoBase {
  
  public:

    /// Default constructor
    SPAlgoPi0();

    /// Default destructor
    virtual ~SPAlgoPi0(){}

    /// What to do before event-loop begins
    virtual void ProcessBegin();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Reconstruct(const SPAData &data);

    /// Function executed at end of process
    virtual void ProcessEnd(TFile* fout);

    /// Likelihood of two showers being pi0 daughters
    void Likelihood(const SPAShower& shower_a,
		    const SPAShower& shower_b,
		    double& likelihood,
		    double& mass);

  protected:

    SPAlgoEMPart  _alg_emp;
    //RooGaussian *_pi0_pdf;
    RooAbsPdf *_pi0_pdf;
    RooRealVar  *_pi0_massVar, *_pi0_massMean, *_pi0_massSigma;
    RooDataSet  *_pi0_massData;
    RooGaussian *_pi0_massPdf;

    double _energy_min,   _energy_max;
    double _angle_min,    _angle_max;
    double _vtx_dist_max;
    double _pi0_mean, _pi0_mean_min, _pi0_mean_max;
    double _pi0_sigma, _pi0_sigma_min, _pi0_sigma_max;
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
