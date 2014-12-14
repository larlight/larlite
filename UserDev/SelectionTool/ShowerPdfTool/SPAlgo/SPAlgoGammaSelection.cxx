#ifndef SELECTIONTOOL_SPALGOGAMMASELECTION_CXX
#define SELECTIONTOOL_SPALGOGAMMASELECTION_CXX

#include "SPAlgoGammaSelection.h"

namespace sptool {

  SPAlgoGammaSelection::SPAlgoGammaSelection() 
    : SPAlgoBase()
    , _factory(nullptr)
    , _data(nullptr)
  {
    _name     = "SPAlgoGammaSelection";
    SetGammaMode();
    Reset();
  }

  void SPAlgoGammaSelection::Reset()
  {
    if(_factory) delete _factory;
    _factory = new ShowerPdfFactory();
    _vars.reserve(2);
    _vars.push_back(RooRealVar("_x","Distance [cm]",xmin,xmax));
    _vars.push_back(RooRealVar("_l","Radiation length [cm]",lmin,lmax));
    _pdf = _factory->RadLenPdf(_vars);
    if(_data) delete _data;
    _data = new RooDataSet(_name.c_str(),"RooFit distance data set",RooArgSet(_vars[0]));
  }
  
  SPArticleSet SPAlgoGammaSelection::Select(const SPAData &data)
  {
    return SPArticleSet();
  }

  void SPAlgoGammaSelection::Fill(const SPAData &data)
  {
    for ( auto &a : data._showers ){
      double x = a.Start().Dist(data._vtxs[0]);
      std::cout << "Distance: " << x << std::endl;
      _vars[0].setVal(x);
      _data->fill();
    }
  }

}

#endif
