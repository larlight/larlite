#ifndef SELECTIONTOOL_SPALGOPI0_CXX
#define SELECTIONTOOL_SPALGOPI0_CXX

#include "SPAlgoPi0.h"

namespace sptool {

  SPAlgoPi0::SPAlgoPi0() 
    : SPAlgoBase()
    ,  _radLenPdf(nullptr)
    , _radLenCorrelationPdf(nullptr)
    , _data(nullptr)
  {
    std::cout<<"Constructor"<<std::endl;
    _name     = "SPAlgoPi0";
    _xmin = 0;
    _xmax = 100;
    _lmin = -1;
    _lmax = 0;
    _rad_len = -0.1;
    Reset();
    std::cout<<"Constructor end"<<std::endl;
  }

  void SPAlgoPi0::LoadParams(std::string fname, size_t version){

    //Load user_info
    SPTBase::LoadParams(fname,version);
    // Extract if parameters found
    if (_params.exist_darray("rad_range")) {
      auto darray = _params.get_darray("rad_range");
      _xmin = (*darray)[0];
      _xmax = (*darray)[1];
    }
    if (_params.exist_darray("rad_len")) {
      auto darray = _params.get_darray("rad_len");
      _rad_len = (*darray)[0];
    }
    // Use radiation length to build 2D rad-length correlation PDF
    delete _radLenCorrelationPdf;
    _radLenCorrelationPdf = new RooProdPdf("radLenCorr","Pi0 Showers Radiation Length Correlation", RooArgSet(*_radLenPdf,*_radLenPdf));

    return;
  }

  void SPAlgoPi0::Reset()
  {
    std::cout<<"Calling "<<__FUNCTION__<<std::endl;
    _radLenRange = new RooRealVar("_x","Distance [cm]",_xmin,_xmax);
    _radLenVal   = new RooRealVar("_l","Radiation length [cm]",_rad_len,_lmin,_lmax);
    delete _radLenPdf;
    _radLenPdf = _factory.RadLenPdf(*_radLenRange,*_radLenVal);
    delete _radLenCorrelationPdf;
    _radLenCorrelationPdf = new RooProdPdf("radLenCorr","Pi0 Showers Radiation Length Correlation", RooArgSet(*_radLenPdf,*_radLenPdf));
    delete _data;
    _data = new RooDataSet("radLenData","RooFit Radiation Length Data",RooArgSet(_vars[0]));
    std::cout<<"Called "<<__FUNCTION__<<std::endl;
  }

  SPArticleSet SPAlgoPi0::Select(const SPAData &data)
  { return SPArticleSet(); }

  void SPAlgoPi0::Fill(const SPAData &data)
  { 

    // Loop over all showers in the event and get Rad. Length info
    // Assume these files come from a sample of photon showers
    for ( auto &s : data._showers ){
      double d = s.Start().Dist(data._vtxs[0]);
      _vars[0].setVal(d);
      _data->add(RooArgSet(_vars[0]));
    }

  }

  void SPAlgoPi0::ProcessEnd(TFile* fout, bool select){
    
    // Fit to radiation length & plot
    RooPlot* frame = nullptr;
    RooFitResult* fit_res = nullptr;
    fit_res = _radLenPdf->fitTo(*_data,RooFit::Save());
    fit_res->Print();
    frame = _vars[0].frame();
    _data->plotOn(frame);
    _radLenPdf->plotOn(frame);
    
    // Save info
    if (_params.exist_darray("rad_range"))
      _params.get_darray("rad_range")->clear();
    _params.append("rad_range",_xmin);
    _params.append("rad_range",_xmax);
    if (_params.exist_darray("rad_len"))
      _params.get_darray("rad_len")->clear();
    _params.append("rad_len",_rad_len);

    TCanvas *c = new TCanvas("c","Radiation Length PDF for Showers",1000,500);
    frame->Draw();
    c->SaveAs("RadLength.png");
   
    return;
  }

}

#endif
