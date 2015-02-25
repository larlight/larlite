//
// Example C++ routine to run your analysis module, LArUtil.
//

#include "LArUtil/Geometry.h"
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TSystem.h>

int main(int argc, char** argv){

  gSystem->Load("libBase");
  TFile* fout = TFile::Open("tree_Geometry.root","RECREATE");
  TTree* tree = new TTree("Geometry","");

  Double_t fDetLength;
  Double_t fDetHalfWidth;
  Double_t fDetHalfHeight;
  tree->Branch("fDetLength",&fDetLength,"fDetLength/D");
  tree->Branch("fDetHalfWidth",&fDetHalfWidth,"fDetHalfWidth/D");
  tree->Branch("fDetHalfHeight",&fDetHalfHeight,"fDetHalfHeight/D");
  
  Double_t fCryoLength;
  Double_t fCryoHalfWidth;
  Double_t fCryoHalfHeight;
  tree->Branch("fCryoLength",&fCryoLength,"fCryoLength/D");
  tree->Branch("fCryoHalfWidth",&fCryoHalfWidth,"fCryoHalfWidth/D");
  tree->Branch("fCryoHalfHeight",&fCryoHalfHeight,"fCryoHalfHeight/D");
  
  std::vector<UChar_t>                fChannelToPlaneMap;
  std::vector<UShort_t>               fChannelToWireMap;
  std::vector<std::vector<UShort_t> > fPlaneWireToChannelMap;
  tree->Branch("fChannelToPlaneMap","std::vector<UChar_t>",&fChannelToPlaneMap);
  tree->Branch("fChannelToWireMap","std::vector<UShort_t>",&fChannelToWireMap);
  tree->Branch("fPlaneWireToChannelMap","std::vector<std::vector<UShort_t> >",&fPlaneWireToChannelMap);
  
  // Vectors with length = # planes
  std::vector<larlite::geo::SigType_t> fSignalType;
  std::vector<larlite::geo::View_t> fViewType;
  std::vector<Double_t> fPlanePitch;
  std::vector<std::vector<Double_t> > fFirstWireStartVtx;
  std::vector<std::vector<Double_t> > fFirstWireEndVtx;
  tree->Branch("fFirstWireStartVtx","std::vector<std::vector<Double_t> >",&fFirstWireStartVtx);
  tree->Branch("fFirstWireEndVtx","std::vector<std::vector<Double_t> >",&fFirstWireEndVtx);
  tree->Branch("fSignalType","std::vector<larlite::geo::SigType_t>",&fSignalType);
  tree->Branch("fViewType","std::vector<larlite::geo::View_t>",&fViewType);
  tree->Branch("fPlanePitch","std::vector<Double_t>",&fPlanePitch);
  
  // Vectors with length = view
  std::vector<Double_t> fWirePitch;
  std::vector<Double_t> fWireAngle;
  tree->Branch("fWirePitch","std::vector<Double_t>",&fWirePitch);
  tree->Branch("fWireAngle","std::vector<Double_t>",&fWireAngle);
  
  std::vector<std::vector<Float_t> > fOpChannelVtx;
  tree->Branch("fOpChannelVtx","std::vector<std::vector<Float_t> >",&fOpChannelVtx);

  std::vector<std::vector<Double_t> > fPlaneOriginVtx;
  tree->Branch("fPlaneOriginVtx","std::vector<std::vector<Double_t> >",&fPlaneOriginVtx);

  tree->Fill();
  tree->Write();
  fout->Close();

  return 0;
}
