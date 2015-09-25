void LowEPlot_nue(){

  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(0000);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(0);
    

  TH1D *nue    = new TH1D("NuEInt",";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *numu   = new TH1D("NuMu",  ";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *nc     = new TH1D("NC",    ";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *cosmic = new TH1D("Cos",   ";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *dirt   = new TH1D("Dirt",  ";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);


  THStack *LEE = new THStack("lee", ";Reconstructed Neutrino Energy [GeV];Events");

  TTree *tree;  
  Int_t           _numEvts;
  Int_t           _neutrinos;
  Double_t        _e_nuReco;
  Double_t        _e_dep;
  Double_t        _weight;
  TBranch        *b_numEvts;   //!
  TBranch        *b_neutrinos;   //!
  TBranch        *b_e_nuReco;   //!
  TBranch        *b_e_dep;   //!
  TBranch        *b_weight;   //!

   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nue_selection.root");
  if (!f || !f->IsOpen()) {
    f = new TFile("singleE_nue_selection.root");
  }

  f->GetObject("beamNuE",tree);

  tree->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree->SetBranchAddress("_weight", &_weight, &b_weight);





  Int_t nentries = tree->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree->GetEntry(i);
    

    nue->Fill(_e_nuReco, _weight*(6.6e20/(2.701e17*1991)));
       
  }

  nue->SetFillColor(kRed);
  
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_mu_selection.root");
  if (!f || !f->IsOpen()) {
    f = new TFile("singleE_mu_selection.root");
  }

  f->GetObject("beamNuE",tree);

  tree->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree->GetEntry(i);
    

    numu->Fill(_e_nuReco, _weight*(6.6e20/(2.701e17*1991)));
       
  }

  numu->SetFillColor(kBlue);

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nc_selection.root");
  if (!f || !f->IsOpen()) {
    f = new TFile("singleE_nc_selection.root");
  }

  f->GetObject("beamNuE",tree);

  tree->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree->GetEntry(i);
    

    nc->Fill(_e_nuReco, _weight*(6.6e20/(2.701e17*1991)));
       
  }

  nc->SetFillColor(kGreen);

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_cosmic_selection.root");
  if (!f || !f->IsOpen()) {
    f = new TFile("singleE_cosmic_selection.root");
  }

  f->GetObject("cosmicShowers",tree);

  tree->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree->GetEntry(i);
    

    cosmic->Fill(_e_nuReco, 2.5);//21);
       
  }

  //  cosmic->Draw("same");
  cosmic->SetFillColor(kBlack);

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_dirt_selection.root");
  if (!f || !f->IsOpen()) {
    f = new TFile("singleE_dirt_selection.root");
  }

  f->GetObject("beamNuE",tree);

  tree->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree->GetEntry(i);
    

    dirt->Fill(_e_nuReco, _weight);
       
  }

  //  dirt->Draw("same");

  LEE->Add(nue);
  LEE->Add(numu);
  LEE->Add(nc);
  LEE->Add(cosmic);

  LEE->Draw();
  
  TLegend *leg = new TLegend(0.5,0.65,0.8,0.9,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->SetTextFont(62);
  leg->SetTextAlign(12);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->AddEntry(nue,"Beam Intrinsic #nu_{e}","f");
  leg->AddEntry(numu,"Beam Intrinsic #nu_{#mu}","f");
  leg->AddEntry(nc,"Beam Intrinsic Neutral Current","f");
  leg->AddEntry(cosmic,"Cosmic, in- and out-time","f");
  leg->Draw();


}
