void LowEPlot_ncCompare(){

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
    

  TH1D *nc_tot = new TH1D("NCtot"  ,";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *charPi = new TH1D("NCpi"   ,";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *gamma  = new TH1D("NCgam"  ,";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);
  TH1D *other  = new TH1D("NCother",";Reconstructed Neutrino Energy [GeV];Events",25, 50, 3000);

  THStack *LEE = new THStack("lee", ";Reconstructed Neutrino Energy [GeV];Events");

  TTree *tree1;  
  TTree *tree2;  
  TTree *tree3;  
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

  TFile *f1 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nc_selection.root");
  if (!f1 || !f1->IsOpen()) {
    f1 = new TFile("singleE_nc_selection.root");
  }
  
  f1->GetObject("beamNC",tree1);
  
  tree1->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree1->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree1->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree1->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree1->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree1->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){    
    nbytes += tree1->GetEntry(i);
    nc_tot->Fill(_e_nuReco);       
  }

  nc_tot->SetLineColor(kRed);
  nc_tot->SetLineWidth(3);

  TFile *f2 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nc_chargedPion_selection.root");
  if (!f2 || !f2->IsOpen()) {
    f2 = new TFile("singleE_nc_chargedPion_selection.root");
  }
  
  f2->GetObject("beamNC",tree2);
  
  tree2->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree2->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree2->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree2->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree2->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree2->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){    
    nbytes += tree2->GetEntry(i);
    charPi->Fill(_e_nuReco);       
  }

  charPi->SetFillColor(kBlue);

  TFile *f3 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nc_gamma_selection.root");
  if (!f3 || !f3->IsOpen()) {
    f3 = new TFile("singleE_nc_gamma_selection.root");
  }
  
  f3->GetObject("beamNC",tree3);
  
  tree3->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree3->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree3->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree3->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree3->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree3->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){    
    nbytes += tree3->GetEntry(i);
    gamma->Fill(_e_nuReco);       
  }

  gamma->SetFillColor(kBlue+2);
  
  /*  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nc_other_selection.root");
  if (!f || !f->IsOpen()) {
    f = new TFile("singleE_nc_other_selection.root");
  }
  
  f->GetObject("beamNC",tree);
  
  tree->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){    
    nbytes += tree->GetEntry(i);
    other->Fill(_e_nuReco, _weight*(6.6e20/(2.701e17*1991)));       
  }

  other->SetFillColor(kBlue-3);
  */

  LEE->Add(charPi);
  LEE->Add(gamma);
  //  LEE->Add(other);

  LEE->Draw();
  nc_tot->Draw("same");

}
