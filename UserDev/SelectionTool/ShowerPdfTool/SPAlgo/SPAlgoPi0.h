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

    /// Function to evaluate input showers and determine a score
    virtual SPArticleSet Select(const SPAData &data);

    /// Function to fill data sample
    virtual void Fill(const SPAData &data);

    virtual void hello() { std::cout<<"ahoaho"<<std::endl;}

  protected:
    double _xmin, _xmax, _lmin, _lmax;
    ShowerPdfFactory _factory;
    RooRealVars_t _vars;
    RooExponential* _pdf;
    RooDataSet _data;

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
