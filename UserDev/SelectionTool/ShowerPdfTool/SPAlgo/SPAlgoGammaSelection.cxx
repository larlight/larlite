#ifndef SELECTIONTOOL_SPALGOGAMMASELECTION_CXX
#define SELECTIONTOOL_SPALGOGAMMASELECTION_CXX

#include "SPAlgoGammaSelection.h"

namespace sptool {

  SPAlgoGammaSelection::SPAlgoGammaSelection() 
    : SPAlgoBase()
    , _pdf(nullptr)
    , _data(nullptr)
  { 
    SetGammaMode(); 
    Reset();
  }


  void SPAlgoGammaSelection::Reset()
  {
    _vars.clear();
    _vars.reserve(2);
    _vars.push_back(RooRealVar("_x","Distance [cm]",_xmin,_xmax));
    _vars.push_back(RooRealVar("_l","Radiation length [cm]",_lmin,_lmax));
    delete _pdf;
    _pdf = _factory.RadLenPdf(_vars);
    delete _data;
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

  void SPAlgoGammaSelection::ProcessEnd(TFile* fout)
  {
    _pdf->fitTo(*_data);
    
    auto frame = _vars[0].frame();
    _data->plotOn(frame);
    _pdf->plotOn(frame);
    TCanvas *c = new TCanvas("c","Distance PDF",1000,500);
    frame->Draw();
    c->SaveAs("aho.png");

    _params.store("_xmin",_xmin);
    _params.store("_xmax",_xmax);
    _params.store("_lmin",_lmin);
    _params.store("_lmax",_lmax);
  }

}

#endif
