#ifndef SELECTIONTOOL_SPALGOGAMMASELECTION_CXX
#define SELECTIONTOOL_SPALGOGAMMASELECTION_CXX

#include "SPAlgoGammaSelection.h"

namespace sptool {

  SPAlgoGammaSelection::SPAlgoGammaSelection() 
    : SPAlgoBase()
    , _e_pdf(nullptr)
    , _e_data(nullptr)
    , _g_pdf(nullptr)
    , _g_data(nullptr)
  { 
    _name = "SPAlgoGammaSelection"; 
    Init();
  }

  void SPAlgoGammaSelection::Init()
  {
    _xmin = 0.;
    _xmax = 100.;
    _e_lmin = -0.1;
    _e_lmax = -0.01;
    _g_lmin = -100.;
    _g_lmax = -10.;
  }

  void SPAlgoGammaSelection::LoadParams(std::string fname,size_t version)
  {
    // Load user_info
    SPTBase::LoadParams(fname,version);
    // Extract if parameters found
    if(_params.exist_darray("rad_range")) {
      auto darray = _params.get_darray("rad_range");
      _xmin = (*darray)[0];
      _xmax = (*darray)[1];
    }
    if(_params.exist_darray("gamma_params")) {
      auto darray = _params.get_darray("gamma_params");
      _g_lmin = (*darray)[0];
      _g_lmax = (*darray)[1];
      std::cout<<"Loaded gamma parameters    : "<<_g_lmin<<" => "<<_g_lmax<<std::endl;
    }
    if(_params.exist_darray("electron_params")) {
      auto darray = _params.get_darray("electron_params");
      _e_lmin = (*darray)[0];
      _g_lmax = (*darray)[1];
      std::cout<<"Loaded electron parameters : "<<_g_lmin<<" => "<<_g_lmax<<std::endl;
    }
  }

  void SPAlgoGammaSelection::Reset()
  {
    ShowerPdfFactory factory;
    _e_vars.clear();
    _e_vars.reserve(2);
    _e_vars.push_back(RooRealVar("_x","Distance [cm]",_xmin,_xmax));
    _e_vars.push_back(RooRealVar("_l","Radiation length [cm]",_e_lmin,_e_lmax));
    delete _e_pdf;
    _e_pdf = factory.RadLenPdf(_e_vars);
    delete _e_data;
    _e_data = new RooDataSet(_name.c_str(),"RooFit distance data set",RooArgSet(_e_vars[0]));

    _g_vars.clear();
    _g_vars.reserve(2);
    _g_vars.push_back(RooRealVar("_x","Distance [cm]",_xmin,_xmax));
    _g_vars.push_back(RooRealVar("_l","Radiation length [cm]",_g_lmin,_g_lmax));
    delete _g_pdf;
    _g_pdf = factory.RadLenPdf(_g_vars);
    delete _g_data;
    _g_data = new RooDataSet(_name.c_str(),"RooFit distance data set",RooArgSet(_g_vars[0]));    
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
      if(_mode) {
	_g_vars[0].setVal(x);
	_g_data->fill();
      }else{
	_e_vars[0].setVal(x);
	_e_data->fill();
      }
    }
  }

  void SPAlgoGammaSelection::ProcessEnd(TFile* fout)
  {

    RooPlot* frame = nullptr;

    if(_mode) {
      _g_pdf->fitTo(*_g_data);    
      frame = _g_vars[0].frame();
      _g_data->plotOn(frame);
      _g_pdf->plotOn(frame);
      if(_params.exist_darray("gamma_params")) 
	_params.get_darray("gamma_params")->clear();
      _params.append("gamma_params",_g_lmin);
      _params.append("gamma_params",_g_lmax);
    }else{
      _e_pdf->fitTo(*_e_data);    
      frame = _e_vars[0].frame();
      _e_data->plotOn(frame);
      _e_pdf->plotOn(frame);
      if(_params.exist_darray("electron_params")) 
	_params.get_darray("electron_params")->clear();
      _params.append("electron_params",_e_lmin);
      _params.append("electron_params",_e_lmax);
    }
    if(_params.exist_darray("rad_range"))
       _params.get_darray("rad_range")->clear();
    _params.append("rad_range",_xmin);
    _params.append("rad_range",_xmax);

    TCanvas *c = new TCanvas("c","Distance PDF",1000,500);
    frame->Draw();
    c->SaveAs("aho.png");

  }

}

#endif
