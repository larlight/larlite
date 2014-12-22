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

#include "SPTBase/SPAlgoBase.h"
#include "SPTBase/ShowerPdfFactory.h"
#include <RooProdPdf.h> 
#include <RooPlot.h>
#include <TCanvas.h>

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
    virtual ~SPAlgoPi0(){_vars.clear();}

    virtual void Reset();

    /// Override the sptool::SPTBase::LoadParams function
    virtual void LoadParams(std::string fname="",size_t version=kINVALID_SIZE);

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Select(const SPAData &data);

    /// Function to fill data sample
    virtual void Fill(const SPAData &data);

    /// Function executed at end of process
    virtual void ProcessEnd(TFile* fout);

    virtual void hello() { std::cout<<"ahoaho"<<std::endl;}

  protected:
    double _xmin, _xmax, _lmin, _lmax;
    double _rad_len;
    ShowerPdfFactory _factory;
    RooRealVar*  _radLenRange;
    RooRealVar*  _radLenVal;
    RooRealVars_t _vars;
    RooExponential* _radLenPdf;
    RooDataSet* _data;

    // PDF for correlation between rad-length
    // of two pi0 showers
    RooProdPdf* _radLenCorrelationPdf;

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
