void LowEPlot_nue(){

  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(0000);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(0);

  TCanvas* c = new TCanvas("c","",1100,1700);
  c->cd();
  
  TH1 *hr = new TH2F("hr","",100,.100,3,100,0,725);
  hr->SetStats(0);
  hr->GetXaxis()->SetTitle("Reconstructed Neutrino Energy [GeV]");
  hr->GetXaxis()->SetLabelFont(42);
  hr->GetXaxis()->SetLabelSize(0.04);
  hr->GetXaxis()->SetTitleSize(0.06);
  hr->GetXaxis()->SetTitleOffset(1.1);
  hr->GetXaxis()->CenterTitle();
  hr->GetYaxis()->SetTitle("Events");
  hr->GetYaxis()->SetNdivisions(509);
  hr->GetXaxis()->SetNdivisions(515);
  hr->GetYaxis()->SetLabelFont(42);
  hr->GetYaxis()->SetLabelOffset(0.01);
  hr->GetYaxis()->SetLabelSize(0.04);
  hr->GetYaxis()->SetTitleSize(0.07);
  hr->GetYaxis()->SetTitleOffset(0.7);
  hr->GetYaxis()->CenterTitle();
  //  hr->GetYaxis()->SetRangeUser(0,1);
  //  hr->SetMaximum((200*sin2));
  hr->Draw();
    


  TH1D *nue    = new TH1D("NuEInt",";Reconstructed Neutrino Energy [GeV];Events",39, .1, 3);
  TH1D *numu   = new TH1D("NuMu",  ";Reconstructed Neutrino Energy [GeV];Events",39, .1, 3);
  TH1D *nc     = new TH1D("NC",    ";Reconstructed Neutrino Energy [GeV];Events",39, .1, 3);
  TH1D *cosmic = new TH1D("Cos",   ";Reconstructed Neutrino Energy [GeV];Events",39, .1, 3);
  TH1D *dirt   = new TH1D("Dirt",  ";Reconstructed Neutrino Energy [GeV];Events",39, .1, 3);


  THStack *LEE = new THStack("lee", ";Reconstructed Neutrino Energy [GeV];Events");

  TTree *tree1;  
  TTree *tree2;  
  TTree *tree3;  
  TTree *tree4;  
  TTree *tree5;  

  Int_t           _numEvts;
  Int_t           _neutrinos;
  Double_t        _e_nuReco;
  Double_t        _e_dep;
  Double_t        _weight; // right now this is the weight from fluxRW package
  TBranch        *b_numEvts;   //!
  TBranch        *b_neutrinos;   //!
  TBranch        *b_e_nuReco;   //!
  TBranch        *b_e_dep;   //!
  TBranch        *b_weight;   //!

   TFile *f1 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nue_selection.root");
  if (!f1 || !f1->IsOpen()) {
    f1 = new TFile("singleE_nue_selection.root");
  }

  f1->GetObject("beamNuE",tree1);
  if(!tree1){
    std::cerr<<"Wasn't able to find tree in file."<<std::endl;
    exit(0);
  }
  tree1->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree1->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree1->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree1->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree1->SetBranchAddress("_weight", &_weight, &b_weight);


  Int_t nentries = tree1->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree1->GetEntry(i);
    
    ///1991 is # of events in the file, 2.701e17 is average POT per event (from POT tree), 6.6e20 is total POT we're scaling to
    nue->Fill(_e_nuReco/1000, _weight*(6.6e20/(2.701e17*1991)));
       
  }

  std::cout << "Nu E : " << nue->Integral() << std::endl;

  nue->SetFillColor(kGreen-2);
  
  TFile *f2 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_mu_selection.root");
  if (!f2 || !f2->IsOpen()) {
    f2 = new TFile("singleE_mu_selection.root");
  }
  f2->GetObject("beamNuMu",tree2);
  if(!tree2){
    std::cerr<<"Wasn't able to find tree in file."<<std::endl;
    exit(0);
  }

  tree2->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree2->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree2->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree2->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree2->SetBranchAddress("_weight", &_weight, &b_weight);

  nentries = tree2->GetEntries();
  nbytes = 0;
  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree2->GetEntry(i);
    
    ///1991 is # of events in the file, 2.701e17 is average POT per event (from POT tree), 6.6e20 is total POT we're scaling to
    numu->Fill(_e_nuReco/1000, _weight*(6.6e20/(2.701e17*1991)));
       
  }

  numu->SetFillColor(kBlue-5);

  TFile *f3 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nc_selection.root");
  if (!f3 || !f3->IsOpen()) {
    f3 = new TFile("singleE_nc_selection.root");
  }

  f3->GetObject("beamNC",tree3);
  if(!tree3){
    std::cerr<<"Wasn't able to find tree in file."<<std::endl;
    exit(0);
  }
  tree3->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree3->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree3->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree3->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree3->SetBranchAddress("_weight", &_weight, &b_weight);

  nentries = tree3->GetEntries();
  nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree3->GetEntry(i);
    
    ///1991 is # of events in the file, 2.701e17 is average POT per event (from POT tree), 6.6e20 is total POT we're scaling to
    nc->Fill(_e_nuReco/1000, _weight*(6.6e20/(2.701e17*1991)));
       
  }

  nc->SetFillColor(kBlue-9);

  TFile *f4 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_cosmic_selection.root");
  if (!f4 || !f4->IsOpen()) {
    f4 = new TFile("singleE_cosmic_selection.root");
  }

  f4->GetObject("cosmicShowers",tree4);
  if(!tree4){
    std::cerr<<"Wasn't able to find tree in file."<<std::endl;
    exit(0);
  }

  tree4->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree4->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree4->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree4->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree4->SetBranchAddress("_weight", &_weight, &b_weight);

  nentries = tree4->GetEntries();
  nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree4->GetEntry(i);
    
    // 2.5 comes from (4.8ms per event * total number of events) / 211 seconds
    // 211 seconds is total amount of beam-gate-window open time that corresponds to 6.6e20 total POT
    // note: this assumes 100% perfect flash-matching (IE no cosmics that arrive outside of the
    // beam gate window get MID-ed as single electron events)
    // note: for the 128 kV files, 2.5 is an overestimate. We generate *four* readout frames worth
    // of cosmics (with a front-porch readout frame so the detector starts fully populated when readout begins)
    // but we only read out 3 frames. therefore, 2.5 needs to be multiplied by (4.8/6.4) = 0.75
    // so 2.5 should be replaced with 1.875
    cosmic->Fill(_e_nuReco/1000, 2.5);//21);
       
  }

  //  cosmic->Draw("same");
  cosmic->SetFillColor(kRed-3);
  /*
  TFile *f5 = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_dirt_selection.root");
  if (!f5 || !f5->IsOpen()) {
    f5 = new TFile("singleE_dirt_selection.root");
  }

  f5->GetObject("beamNuE",tree5);
  if(!tree5){
    std::cerr<<"Wasn't able to find tree in file."<<std::endl;
    exit(0);
  }
  tree5->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
  tree5->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
  tree5->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
  tree5->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
  tree5->SetBranchAddress("_weight", &_weight, &b_weight);

  Int_t nentries = tree5->GetEntries();
  Int_t nbytes = 0;

  for(Int_t i = 0; i < nentries; i++){
    
    nbytes += tree5->GetEntry(i);
    

    dirt->Fill(_e_nuReco/1000, _weight);
       
  }
  */
  //  dirt->Draw("same");

  LEE->Add(nue);
  LEE->Add(numu);
  LEE->Add(nc);
  LEE->Add(cosmic);

  LEE->Draw("samehist");
  
  c->RedrawAxis();
  
  
  TLegend *leg = new TLegend(0.3,0.65,0.6,0.9,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->SetTextFont(62);
  leg->SetTextAlign(12);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->AddEntry(cosmic,TString::Format("Cosmic, in-time : %4.1f Events", cosmic->Integral() ),"f");
  leg->AddEntry(numu,TString::Format("Beam Intrinsic #nu_{#mu} : %4.1f Events", numu->Integral() ),"f");
  leg->AddEntry(nc,TString::Format("Beam Intrinsic Neutral Current : %4.1f Events", nc->Integral() ),"f");
  leg->AddEntry(nue,TString::Format("Beam Intrinsic #nu_{e} : %4.1f Events", nue->Integral() ),"f");
  leg->Draw();


}
