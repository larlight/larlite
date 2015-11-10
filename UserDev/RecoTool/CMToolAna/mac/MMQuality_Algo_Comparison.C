///////////////////////////////////////////////////////
// This macro just saves the 
// Efficiency vs Purity plot per plan
// Purity     vs # of Reconstructed hits plot per plan
// Efficiency vs # of Reconstructed hits plot per plan
// With the input file name 
///////////////////////////////////////////////////////

#include "TInterpreter.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TH1.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TPaveStats.h"
#include "TFrame.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>

void MMQuality_Algo_Comparison( std::string dirName = "/uboone/app/users/elena/LArlite/larlite/MergeAnaCombo/Temp/MMQualityFuzzyFiles/", 
				std::string inFileName ="MMQualityFiles.db", 
				bool SaveMe = 0)
{
  TFile   *inFile;
  //gStyle->SetOptStat(0);  
  //#############################################################################################################
  //###########################################  Read From File #################################################
  //#############################################################################################################

  std::string line;
  //std::string dirName = "/uboone/app/users/elena/LArlite/larlite/MergeAnaCombo/Temp/MMQualityFiles/";
  std::ifstream myfile  (inFileName.c_str());
  std::ifstream myfile2 (inFileName.c_str());
  Int_t lineInFileName = 0;
  std::string line_bogus;
  
  while (std::getline(myfile2, line_bogus))  ++lineInFileName;
  std::cout << "Number of lines in text file: " << lineInFileName<<"\n";


  TCanvas *C0     = new TCanvas("C0","C0",1400,800);
  C0->Divide(3,2);
  
  //Define Figure of Merit Histograms
  auto hFigOfMeritEPlan0 = new TH1D("figOfMeritEPlan0","Efficiency Plane 0; Combo; Fig of Merit Eff",lineInFileName,-0.5,(double)lineInFileName-0.5);
  auto hFigOfMeritPPlan0 = new TH1D("figOfMeritPPlan0","    Purity Plane 0; Combo; Fig of Merit Pur",lineInFileName,-0.5,(double)lineInFileName-0.5);
  auto hFigOfMeritEPlan1 = new TH1D("figOfMeritEPlan1","Efficiency Plane 1; Combo; Fig of Merit Eff",lineInFileName,-0.5,(double)lineInFileName-0.5);
  auto hFigOfMeritPPlan1 = new TH1D("figOfMeritPPlan1","    Purity Plane 1; Combo; Fig of Merit Pur",lineInFileName,-0.5,(double)lineInFileName-0.5);
  auto hFigOfMeritEPlan2 = new TH1D("figOfMeritEPlan2","Efficiency Plane 2; Combo; Fig of Merit Eff",lineInFileName,-0.5,(double)lineInFileName-0.5);
  auto hFigOfMeritPPlan2 = new TH1D("figOfMeritPPlan2","    Purity Plane 2; Combo; Fig of Merit Pur",lineInFileName,-0.5,(double)lineInFileName-0.5);

  int i = 0;

  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
	{
	  std::cout << "I'm reading file " << line << '\n';
	  std::string rootFileName = dirName;
	  rootFileName.append(line);
	  
	  inFile = new TFile(rootFileName.c_str());
	  // Enter the RootFile
	  inFile->cd();	 
	  TTree* _TTree = (TTree*)(inFile->Get("fMMQualityTree"));

	  //#############################################################################################################
	  //############################################  Efficiency  ###################################################
	  //#############################################################################################################
	  C0->cd(1);  
 	  auto EPlan0 = new TH1D("EPlan0","Efficiency Plane 0; Count ; Efficiency",101,-0.05,1.05);
	  _TTree->Draw("BestEff_0>>EPlan0");
	  C0->Update();  
	  Double_t figOfMeritE0 = EPlan0->Integral(81,101)/EPlan0->Integral();

 	  auto EPlan1 = new TH1D("EPlan1","Efficiency Plane 1; Count ; Efficiency",101,-0.05,1.05);
	  _TTree->Draw("BestEff_1>>EPlan1");
	  C0->Update();  
	  Double_t figOfMeritE1 = EPlan1->Integral(81,101)/EPlan1->Integral();

 	  auto EPlan2 = new TH1D("EPlan2","Efficiency Plane 2; Count ; Efficiency",101,-0.05,1.05);
	  _TTree->Draw("BestEff_2>>EPlan2");
	  C0->Update();  
	  Double_t figOfMeritE2 = EPlan2->Integral(81,101)/EPlan2->Integral();

	  //#############################################################################################################
	  //##############################################  Purity  #####################################################
	  //#############################################################################################################
	  C0->cd(2);  
	  auto PPlan0 = new TH1D("PPlan0","Purity Plane 0; Count ; Purity",101,-0.05,1.05);
	  _TTree->Draw("BestPur_0>>PPlan0");
	  C0->Update();  
	  double figOfMeritP0 = (double) PPlan0->Integral(81,101)/((double)PPlan0->Integral());

	  auto PPlan1 = new TH1D("PPlan1","Purity Plane 1; Count ; Purity",101,-0.05,1.05);
	  _TTree->Draw("BestPur_1>>PPlan1");
	  C0->Update();  
	  double figOfMeritP1 = (double) PPlan0->Integral(81,101)/((double)PPlan1->Integral());

	  auto PPlan2 = new TH1D("PPlan2","Purity Plane 2; Count ; Purity",101,-0.05,1.05);
	  _TTree->Draw("BestPur_2>>PPlan2");
	  C0->Update();  
	  double figOfMeritP2 = (double) PPlan2->Integral(81,101)/((double)PPlan2->Integral());


	  //Fill the Comparison histograms
	  i++;
	  std::cout<<"Bin : "<<i<<"\n";
	  hFigOfMeritEPlan0->SetBinContent(i,figOfMeritE0);
	  hFigOfMeritPPlan0->SetBinContent(i,figOfMeritP0);
	  hFigOfMeritEPlan1->SetBinContent(i,figOfMeritE1);
	  hFigOfMeritPPlan1->SetBinContent(i,figOfMeritP1);
	  hFigOfMeritEPlan2->SetBinContent(i,figOfMeritE2);
	  hFigOfMeritPPlan2->SetBinContent(i,figOfMeritP2);

  
	  delete inFile;
	}
      myfile.close();
    }  else std::cout << "Unable to open file\n"; 

  hFigOfMeritEPlan0->SetMarkerStyle(20);
  hFigOfMeritPPlan0->SetMarkerStyle(20);
  hFigOfMeritEPlan1->SetMarkerStyle(20);
  hFigOfMeritPPlan1->SetMarkerStyle(20);
  hFigOfMeritEPlan2->SetMarkerStyle(20);
  hFigOfMeritPPlan2->SetMarkerStyle(20);

  C0->cd(1);  
  hFigOfMeritEPlan0->Draw("P");
  C0->cd(2);  
  hFigOfMeritEPlan1->Draw("P");
  C0->cd(3);  
  hFigOfMeritEPlan2->Draw("P");
  C0->cd(4); 
  hFigOfMeritPPlan0->Draw("P");
  C0->cd(5); 
  hFigOfMeritPPlan1->Draw("P");
  C0->cd(6); 
  hFigOfMeritPPlan2->Draw("P");


  if (SaveMe) 
    {
      C0->SaveAs("EffAndPurComparison.png"); 
    }

  /*



  ////////////// Plan 0 /////////////////////// 

  C0->cd(1);  
  auto EPlan0 = new TH1D("EPlan0","Efficiency Plane 0; Count ; Efficiency",101,-0.05,1.05);
  _TTree->Draw("BestEff_0>>EPlan0","","colz");
  C0->Update();  
  Double_t figOfMeritE = EPlan0->Integral(81,101)/EPlan0->Integral();
  auto hFigOfMeritEPlan0 = new TH1D("figOfMeritEPlan0","Efficiency Plane 0; Combo; Fig of Merit Eff",10,0,10);
  hFigOfMeritEPlan0->SetBinContent(1,figOfMeritE);
  C0->Update();  

  C0->cd(2);  
  auto PPlan0 = new TH1D("PPlan0","Purity Plane 0; Count ; Purity",101,-0.05,1.05);
  _TTree->Draw("BestPur_0>>PPlan0","","colz");
  C0->Update();  
  double figOfMeritP = (double) PPlan0->Integral(81,101)/((double)PPlan0->Integral());
  auto hFigOfMeritPPlan0 = new TH1D("figOfMeritPPlan0","Efficiency Plane 0; Combo; Fig of Merit Eff",10,0,10);
  hFigOfMeritPPlan0->SetBinContent(1,figOfMeritP);

  C0->cd(1);  
  hFigOfMeritEPlan0->SetMarkerSize(20);
  hFigOfMeritEPlan0->SetMarkerStyle(20);
  hFigOfMeritEPlan0->Draw();

  C0->cd(2);  
  hFigOfMeritPPlan0->SetMarkerSize(20);
  hFigOfMeritPPlan0->SetMarkerStyle(20);
  hFigOfMeritPPlan0->Draw();
  C0->Update();  

  ////////////// Plan 1 /////////////////////// 
  TCanvas *C1     = new TCanvas("C1","C1",400,800);
  C1->Divide(1,2);
  ////////////// Plan 2 /////////////////////// 
  TCanvas *C2     = new TCanvas("C2","C2",400,800);
  C2->Divide(1,2);
  
  //Are we saving this plots?
  */
  
  
}


