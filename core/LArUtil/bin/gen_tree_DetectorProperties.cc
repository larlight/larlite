//
// Example C++ routine to run your analysis module, LArUtil.
//

#include "LArUtil/LArUtil-TypeDef.h"
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TSystem.h>

int main(int argc, char** argv){

  gSystem->Load("libBase");
  TFile *fout = TFile::Open("tree_DetectorProperties.root","RECREATE");
  
  TTree* tree = new TTree("DetectorProperties","");
  
  Double_t fSamplingRate;      ///< in ns
  Int_t    fTriggerOffset;     ///< in # of clock ticks
  Double_t fElectronsToADC;    ///< conversion factor for # of ionization electrons to 1 ADC count
  UInt_t   fNumberTimeSamples; ///< number of clock ticks per event
  UInt_t   fReadOutWindowSize; ///< number of clock ticks per readout window
  Double_t fTimeOffsetU;       ///< time offsets to convert spacepoint
  Double_t fTimeOffsetV;       ///< coordinates to hit times on each
  Double_t fTimeOffsetZ;       ///< view
  
  tree->Branch("fSamplingRate",&fSamplingRate,"fSamplingRate/D");
  tree->Branch("fTriggerOffset",&fTriggerOffset,"fTriggerOffset/I");
  tree->Branch("fElectronsToADC",&fElectronsToADC,"fElectronsToADC/D");
  tree->Branch("fNumberTimeSamples",&fNumberTimeSamples,"fNumberTimeSamples/i");
  tree->Branch("fReadOutWindowSize",&fReadOutWindowSize,"fReadOutWindowSize/i");
  tree->Branch("fTimeOffsetU",&fTimeOffsetU,"fTimeOffsetU/D");
  tree->Branch("fTimeOffsetV",&fTimeOffsetV,"fTimeOffsetV/D");
  tree->Branch("fTimeOffsetZ",&fTimeOffsetZ,"fTimeOffsetZ/D");
  
  Double_t fXTicksCoefficient; ///< Parameters for x<-->ticks
  std::vector<Double_t> fXTicksOffsets;

  tree->Branch("fXTicksCoefficient",&fXTicksCoefficient,"fXTicksCoefficient/D");
  tree->Branch("fXTicksOffsets","std::vector<Double_t>",&fXTicksOffsets);

  tree->Fill();
  tree->Write();
  fout->Close();

  return 0;
}
