#ifndef SELECTIONTOOL_SPALGOEMPART_CXX
#define SELECTIONTOOL_SPALGOEMPART_CXX

#include "SPAlgoEMPart.h"

namespace sptool {

  SPAlgoEMPart::SPAlgoEMPart() 
    : SPAlgoBase()
    , _e_pdf(nullptr)
    , _e_data(nullptr)
    , _g_pdf(nullptr)
    , _g_data(nullptr)
  { 
    _name = "SPAlgoEMPart"; 
    Init();
  }

  void SPAlgoEMPart::Init()
  {
    _xmin = 0.;
    _xmax = 100.;
    _e_lval = -1./0.0001;
    _e_lmin = -1./0.00001;
    _e_lmax = -1./0.01;
    _g_lval = -1./20;
    _g_lmin = -1./10.;
    _g_lmax = -1./100.;
  }

  void SPAlgoEMPart::LoadParams(std::string fname,size_t version)
  {
    // Load user_info
    SPTBase::LoadParams(fname,version);
    // Extract if parameters found
    if(_params.exist_darray("rad_range")) {
      auto darray = _params.get_darray("rad_range");
      _xmin = (*darray)[0];
      _xmax = (*darray)[1];
      std::cout<<"["<<__FUNCTION__<<"]"
	       << " Loaded distance fit range : "
	       << _xmin << " => " << _xmax
	       << std::endl;
    }
    if(_params.exist_darray("gamma_params")) {
      auto darray = _params.get_darray("gamma_params");
      _g_lval = (*darray)[0];
      _g_lmin = (*darray)[1];
      _g_lmax = (*darray)[2];
      std::cout<<"["<<__FUNCTION__<<"]"
	       <<" Loaded gamma parameters    : "
	       <<_g_lval << " ["<<_g_lmin<<" => "<<_g_lmax<<"]"
	       <<std::endl;
    }
    if(_params.exist_darray("electron_params")) {
      auto darray = _params.get_darray("electron_params");
      _e_lval = (*darray)[0];
      _e_lmin = (*darray)[1];
      _e_lmax = (*darray)[2];
      std::cout<<"["<<__FUNCTION__<<"]"
	       <<"Loaded electron parameters : "
	       <<_e_lval << " ["<<_e_lmin<<" => "<<_e_lmax<<"]"
	       <<std::endl;
    }
  }

  void SPAlgoEMPart::Reset()
  {
    ShowerPdfFactory factory;
    std::string part_name = "gamma";
    if(!_mode) part_name = "electron";

    std::cout<<"electron: "<<_e_lval<<" : "<<_e_lmin<<" => "<<_e_lmax<<std::endl;
    _e_vars.clear();
    _e_vars.reserve(2);
    _e_vars.push_back(RooRealVar("_e_x","Distance [cm]",_xmin,_xmax));
    _e_vars.push_back(RooRealVar("_e_l","Radiation length [cm]",_e_lval,_e_lmin,_e_lmax));
    delete _e_pdf;
    _e_pdf = factory.RadLenPdf(_e_vars);
    delete _e_data;
    _e_data = new RooDataSet(Form("%s_data",part_name.c_str()),"RooFit distance data set",RooArgSet(_e_vars[0]));

    std::cout<<"gamma: "<<_g_lval<<" : "<<_g_lmin<<" => "<<_g_lmax<<std::endl;
    _g_vars.clear();
    _g_vars.reserve(2);
    _g_vars.push_back(RooRealVar("_g_x","Distance [cm]",_xmin,_xmax));
    _g_vars.push_back(RooRealVar("_g_l","Radiation length [cm]",_g_lval,_g_lmin,_g_lmax));
    delete _g_pdf;
    _g_pdf = factory.RadLenPdf(_g_vars);
    delete _g_data;
    _g_data = new RooDataSet(Form("%s_data",part_name.c_str()),"RooFit distance data set",RooArgSet(_g_vars[0]));

  }
  
  SPArticleSet SPAlgoEMPart::Select(const SPAData &data)
  {
    SPArticleSet res;
    if(!data._vtxs.size()) return res;

    for(auto const& s : data._showers) {

      double g_like = 0;
      double dist   = s.Start().Dist(data._vtxs[0]);

      if(dist > 1.){
	_g_vars[0].setVal(dist);
	_e_vars[0].setVal(dist);

	g_like = _g_pdf->getVal(_g_vars[0]) / ( _g_pdf->getVal(_g_vars[0]) + _e_pdf->getVal(_e_vars[0]) );
      }

      SPArticle p;
      if(g_like>0.5) p.pdg_code(22);
      else p.pdg_code(11);

      p.pos(s.Start());
      p.mom(s.Dir() * s._energy);
      p.energy(s._energy);
      
      res.push_back(p);
      /*
      std::cout<<_g_vars[0].getVal()<< " ... " << g_like << " = ";
      if(g_like) std::cout<<"gamma"<<std::endl;
      else std::cout<<"electron"<<std::endl;
      */
    }
    
    return res;
  }

  void SPAlgoEMPart::Fill(const SPAData &data)
  {
    for ( auto &a : data._showers ){
      double x = a.Start().Dist(data._vtxs[0]);
      //std::cout << "Distance: " << x << std::endl;
      if(_mode) {
	_g_vars[0].setVal(x);
	_g_data->add(RooArgSet(_g_vars[0]));
      }else{
	_e_vars[0].setVal(x);
	_e_data->add(RooArgSet(_e_vars[0]));
      }
    }
  }

  void SPAlgoEMPart::ProcessEnd(TFile* fout)
  {

    RooPlot* frame = nullptr;
    RooFitResult* fit_res = nullptr;
    std::string part_name = "gamma";
    if(!_mode) part_name = "electron";

    if(_mode) {
      fit_res = _g_pdf->fitTo(*_g_data,RooFit::Save());
      fit_res->Print();
      frame = _g_vars[0].frame();
      _g_data->plotOn(frame);
      _g_pdf->plotOn(frame);
    }else{
      fit_res = _e_pdf->fitTo(*_e_data,RooFit::Save());
      fit_res->Print();
      frame = _e_vars[0].frame();
      _e_data->plotOn(frame);
      _e_pdf->plotOn(frame);
    }

    if(_params.exist_darray(Form("%s_params",part_name.c_str()))) {
      std::cout<<"["<<__FUNCTION__<<Form("] Overwriting the stored param: %s_params",part_name.c_str())<<std::endl;
      _params.get_darray(Form("%s_params",part_name.c_str()))->clear();
    }

    RooRealVar* res_value = nullptr;
    if(_mode)
      res_value = (RooRealVar*)(fit_res->floatParsFinal().find("_g_l"));
    else
      res_value = (RooRealVar*)(fit_res->floatParsFinal().find("_e_l"));
    std::cout<<"["<<__FUNCTION__<<Form("] Storing new %s_params: ",part_name.c_str())
	     << res_value->getVal() << " [" << res_value->getErrorLo()
	     << " => " << res_value->getErrorHi() << "]" << std::endl;
    _params.append(Form("%s_params",part_name.c_str()),res_value->getVal());
    _params.append(Form("%s_params",part_name.c_str()),res_value->getVal()+res_value->getErrorLo());
    _params.append(Form("%s_params",part_name.c_str()),res_value->getVal()+res_value->getErrorHi());
    //_params.dump_contents();
    if(_params.exist_darray("rad_range"))
       _params.get_darray("rad_range")->clear();
    _params.append("rad_range",_xmin);
    _params.append("rad_range",_xmax);

    TCanvas *c = new TCanvas("c",Form("Distance PDF for %s",part_name.c_str()),1000,500);
    frame->Draw();
    c->SaveAs(Form("RadLength_%s.png",part_name.c_str()));

    c->SetTitle("Selection Likelihood");

    TH1D *h11 = new TH1D("h11","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",200,0,0.5);
    TH1D *h22 = new TH1D("h22","Electron vs. Gamma Likelihood; Rad. Length [cm]; Likelihood",200,0,0.5);

    for(size_t i=1; i<=100; ++i) {
      _e_vars[0].setVal(0.5/200.*i);
      _g_vars[0].setVal(0.5/200.*i);

      h11->SetBinContent(i,_e_pdf->getVal(_e_vars[0]) / (_e_pdf->getVal(_e_vars[0]) + _g_pdf->getVal(_g_vars[0])));
      h22->SetBinContent(i,_g_pdf->getVal(_g_vars[0]) / (_e_pdf->getVal(_e_vars[0]) + _g_pdf->getVal(_g_vars[0])));
      /*
      std::cout
	<< _e_vars[0].getVal()
	<< " : "
	<< _e_pdf->getVal(_e_vars[0]) / (_e_pdf->getVal(_e_vars[0]) + _g_pdf->getVal(_g_vars[0]))
	<< " : "
	<< _g_pdf->getVal(_g_vars[0]) / (_e_pdf->getVal(_e_vars[0]) + _g_pdf->getVal(_g_vars[0]))
	<< " ... "
	<< _e_pdf->getVal(_e_vars[0])
	<< " : "
	<< _g_pdf->getVal(_g_vars[0])
	<< std::endl;
      */
    }

    h11->SetLineWidth(2);
    h11->SetLineColor(kBlue);
    h11->SetFillStyle(3004);
    h11->SetFillColor(kBlue);
    h22->SetLineWidth(2);
    h22->SetLineColor(kRed);
    h22->SetFillStyle(3003);
    h22->SetFillColor(kRed);

    h11->Draw();
    h22->Draw("sames");
    c->SaveAs("Likelihood.png");

    delete h11;
    delete h22;
    delete c;

  }

}

#endif
