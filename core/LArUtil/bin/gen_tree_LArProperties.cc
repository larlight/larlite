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
  TFile* fout = TFile::Open("tree_LArProperties.root","RECREATE");

  TTree* data_tree = new TTree("LArProperties","");
  
  std::vector< Double_t >          fEfield;           ///< kV/cm 
  Double_t                         fTemperature;      ///< kelvin
  Double_t                         fElectronlifetime; ///< microseconds
  Double_t                         fRadiationLength;  ///< g/cm^2
  data_tree->Branch("fEfield","std::vector<Double_t>", &fEfield);
  data_tree->Branch("fTemperature",&fTemperature,"fTemperature/D");
  data_tree->Branch("fElectronlifetime",&fElectronlifetime,"fElectronlifetime/D");
  data_tree->Branch("fRadiationLength",&fRadiationLength,"fRadiationLength/D");

  Double_t                         fArgon39DecayRate; ///<  decays per cm^3 per second
  data_tree->Branch("fArgon39DecayRate",&fArgon39DecayRate,"fArgon39DecayRate/D");
  
  // Following parameters are for use in Bethe-Bloch formula for dE/dx.
  Double_t fZ;                ///< Ar atomic number
  Double_t fA;                ///< Ar atomic mass (g/mol)
  Double_t fI;                ///< Ar mean excitation energy (eV)
  Double_t fSa;               ///< Sternheimer parameter a
  Double_t fSk;               ///< Sternheimer parameter k
  Double_t fSx0;              ///< Sternheimer parameter x0
  Double_t fSx1;              ///< Sternheimer parameter x1
  Double_t fScbar;            ///< Sternheimer parameter Cbar
  data_tree->Branch("fZ",&fZ,"fZ/D");
  data_tree->Branch("fA",&fA,"fA/D");
  data_tree->Branch("fI",&fI,"fI/D");
  data_tree->Branch("fSa",&fSa,"fSa/D");
  data_tree->Branch("fSk",&fSk,"fSk/D");
  data_tree->Branch("fSx0",&fSx0,"fSx0/D");
  data_tree->Branch("fSx1",&fSx1,"fSx1/D");
  data_tree->Branch("fScbar",&fScbar,"fScbar/D");
  
  // Optical parameters for LAr 
  std::vector<Double_t> fFastScintSpectrum;
  std::vector<Double_t> fFastScintEnergies;
  std::vector<Double_t> fSlowScintSpectrum;
  std::vector<Double_t> fSlowScintEnergies;
  std::vector<Double_t> fRIndexSpectrum;
  std::vector<Double_t> fRIndexEnergies;
  std::vector<Double_t> fAbsLengthSpectrum;
  std::vector<Double_t> fAbsLengthEnergies;
  std::vector<Double_t> fRayleighSpectrum;
  std::vector<Double_t> fRayleighEnergies;
  data_tree->Branch("fFastScintSpectrum","std::vector<Double_t>",&fFastScintSpectrum);
  data_tree->Branch("fFastScintEnergies","std::vector<Double_t>",&fFastScintEnergies);
  data_tree->Branch("fSlowScintSpectrum","std::vector<Double_t>",&fSlowScintSpectrum);
  data_tree->Branch("fSlowScintEnergies","std::vector<Double_t>",&fSlowScintEnergies);
  data_tree->Branch("fRIndexSpectrum","std::vector<Double_t>",&fRIndexSpectrum);
  data_tree->Branch("fRIndexEnergies","std::vector<Double_t>",&fRIndexEnergies);
  data_tree->Branch("fAbsLengthSpectrum","std::vector<Double_t>",&fAbsLengthSpectrum);
  data_tree->Branch("fAbsLengthEnergies","std::vector<Double_t>",&fAbsLengthEnergies);
  data_tree->Branch("fRayleighSpectrum","std::vector<Double_t>",&fRayleighSpectrum);
  data_tree->Branch("fRayleighEnergies","std::vector<Double_t>",&fRayleighEnergies);
  
  bool fScintByParticleType;
  data_tree->Branch("fScintByParticleType",&fScintByParticleType,"fScintByParticleType/B");
  
  Double_t fProtonScintYield;
  Double_t fProtonScintYieldRatio;
  Double_t fMuonScintYield;
  Double_t fMuonScintYieldRatio;
  Double_t fPionScintYield;
  Double_t fPionScintYieldRatio;
  Double_t fKaonScintYield;
  Double_t fKaonScintYieldRatio;
  Double_t fElectronScintYield;
  Double_t fElectronScintYieldRatio;
  Double_t fAlphaScintYield;
  Double_t fAlphaScintYieldRatio;
  data_tree->Branch("fProtonScintYield",&fProtonScintYield,"fProtonScintYield/D");
  data_tree->Branch("fProtonScintYieldRatio",&fProtonScintYieldRatio,"fProtonScintYieldRatio/D");
  data_tree->Branch("fMuonScintYield",&fMuonScintYield,"fMuonScintYield/D");
  data_tree->Branch("fMuonScintYieldRatio",&fMuonScintYieldRatio,"fMuonScintYieldRatio/D");
  data_tree->Branch("fPionScintYield",&fPionScintYield,"fPionScintYield/D");
  data_tree->Branch("fPionScintYieldRatio",&fPionScintYieldRatio,"fPionScintYieldRatio/D");
  data_tree->Branch("fKaonScintYield",&fKaonScintYield,"fKaonScintYield/D");
  data_tree->Branch("fKaonScintYieldRatio",&fKaonScintYieldRatio,"fKaonScintYieldRatio/D");
  data_tree->Branch("fElectronScintYield",&fElectronScintYield,"fElectronScintYield/D");
  data_tree->Branch("fElectronScintYieldRatio",&fElectronScintYieldRatio,"fElectronScintYieldRatio/D");
  data_tree->Branch("fAlphaScintYield",&fAlphaScintYield,"fAlphaScintYield/D");
  data_tree->Branch("fAlphaScintYieldRatio",&fAlphaScintYieldRatio,"fAlphaScintYieldRatio/D");
  
  Double_t fScintYield;
  Double_t fScintResolutionScale;
  Double_t fScintFastTimeConst;
  Double_t fScintSlowTimeConst;
  Double_t fScintYieldRatio;
  Double_t fScintBirksConstant;
  data_tree->Branch("fScintYield",&fScintYield,"fScintYield/D");
  data_tree->Branch("fScintResolutionScale",&fScintResolutionScale,"fScintResolutionScale/D");
  data_tree->Branch("fScintFastTimeConst",&fScintFastTimeConst,"fScintFastTimeConst/D");
  data_tree->Branch("fScintSlowTimeConst",&fScintSlowTimeConst,"fScintSlowTimeConst/D");
  data_tree->Branch("fScintYieldRatio",&fScintYieldRatio,"fScintYieldRatio/D");  
  data_tree->Branch("fScintBirksConstant",&fScintBirksConstant,"fScintBirksConstant/D");
  
  bool fEnableCerenkovLight;
  data_tree->Branch("fEnableCerenkovLight",&fEnableCerenkovLight,"fEnableCerenkovLight/B");  

  std::vector<std::string>            fReflectiveSurfaceNames;
  std::vector<Double_t>               fReflectiveSurfaceEnergies;
  std::vector<std::vector<Double_t> > fReflectiveSurfaceReflectances;
  std::vector<std::vector<Double_t> > fReflectiveSurfaceDiffuseFractions;
  data_tree->Branch("fReflectiveSurfaceNames","std::vector<std::string>",&fReflectiveSurfaceNames);
  data_tree->Branch("fReflectiveSurfaceEnergies","std::vector<Double_t>",&fReflectiveSurfaceEnergies);
  data_tree->Branch("fReflectiveSurfaceReflectances","std::vector<std::vector<Double_t> >",&fReflectiveSurfaceReflectances);
  data_tree->Branch("fReflectiveSurfaceDiffuseFractions","std::vector<std::vector<Double_t> >",&fReflectiveSurfaceDiffuseFractions);

  data_tree->Fill();
  data_tree->Write();
  fout->Close();

  return 0;
}
