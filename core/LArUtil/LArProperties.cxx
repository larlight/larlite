#ifndef LARLITE_LARPROPERTIES_CXX
#define LARLITE_LARPROPERTIES_CXX

#include "LArProperties.h"

namespace larutil {

  LArProperties* LArProperties::_me = 0;

  LArProperties::LArProperties(bool default_load) : LArUtilBase()
  {
    _name = "LArProperties";
    if(default_load){
      _file_name = Form("%s/LArUtil/dat/%s",
			getenv("LARLITE_COREDIR"),
			kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str());
      _tree_name = kTREENAME_LARPROPERTIES;
      LoadData();
    }
  }

  void LArProperties::ClearData()
  {
    fEfield.clear();
    fTemperature = larlite::data::kINVALID_DOUBLE;
    fVd = larlite::data::kINVALID_DOUBLE;
    fElectronlifetime = larlite::data::kINVALID_DOUBLE;
    fRadiationLength = larlite::data::kINVALID_DOUBLE;
    fArgon39DecayRate = larlite::data::kINVALID_DOUBLE;
    fZ = larlite::data::kINVALID_DOUBLE;
    fA = larlite::data::kINVALID_DOUBLE;
    fI = larlite::data::kINVALID_DOUBLE;
    fSa = larlite::data::kINVALID_DOUBLE;
    fSx0 = larlite::data::kINVALID_DOUBLE;
    fSx1 = larlite::data::kINVALID_DOUBLE;
    fScbar = larlite::data::kINVALID_DOUBLE;
    fFastScintSpectrum.clear();
    fFastScintEnergies.clear();
    fSlowScintSpectrum.clear();
    fSlowScintEnergies.clear();
    fRIndexSpectrum.clear();
    fRIndexEnergies.clear();
    fAbsLengthSpectrum.clear();
    fAbsLengthEnergies.clear();
    fRayleighSpectrum.clear();
    fRayleighEnergies.clear();
    fScintByParticleType=false;
    fProtonScintYield=larlite::data::kINVALID_DOUBLE;
    fProtonScintYieldRatio=larlite::data::kINVALID_DOUBLE;
    fPionScintYield=larlite::data::kINVALID_DOUBLE;
    fPionScintYieldRatio=larlite::data::kINVALID_DOUBLE;
    fMuonScintYield=larlite::data::kINVALID_DOUBLE;
    fMuonScintYieldRatio=larlite::data::kINVALID_DOUBLE;
    fKaonScintYield=larlite::data::kINVALID_DOUBLE;
    fKaonScintYieldRatio=larlite::data::kINVALID_DOUBLE;
    fElectronScintYield=larlite::data::kINVALID_DOUBLE;
    fElectronScintYieldRatio=larlite::data::kINVALID_DOUBLE;
    fAlphaScintYield=larlite::data::kINVALID_DOUBLE;
    fAlphaScintYieldRatio=larlite::data::kINVALID_DOUBLE;
    fScintYield = larlite::data::kINVALID_DOUBLE;
    fScintResolutionScale = larlite::data::kINVALID_DOUBLE;
    fScintFastTimeConst = larlite::data::kINVALID_DOUBLE;
    fScintSlowTimeConst = larlite::data::kINVALID_DOUBLE;
    fScintYieldRatio = larlite::data::kINVALID_DOUBLE;
    fScintBirksConstant = larlite::data::kINVALID_DOUBLE;
    fEnableCerenkovLight = false;
    fReflectiveSurfaceNames.clear();
    fReflectiveSurfaceEnergies.clear();
    fReflectiveSurfaceReflectances.clear();    
    fReflectiveSurfaceDiffuseFractions.clear();
  }

  bool LArProperties::ReadTree()
  {
    ClearData();

    fVd = larutil::kDriftVelMCC9;
    
    TChain* ch = new TChain(_tree_name.c_str());
    ch->AddFile(_file_name.c_str());

    std::string error_msg("");
    if(!(ch->GetBranch("fEfield")))           error_msg += "      fEfield\n";
    if(!(ch->GetBranch("fTemperature")))      error_msg += "      fTemperature\n";
    if(!(ch->GetBranch("fElectronlifetime"))) error_msg += "      fElectronlifetime\n";
    if(!(ch->GetBranch("fRadiationLength")))  error_msg += "      fRadiationLength\n";
    if(!(ch->GetBranch("fArgon39DecayRate"))) error_msg += "      fArgon39DecayRate\n";

    if(!(ch->GetBranch("fZ")))     error_msg += "      fZ\n";
    if(!(ch->GetBranch("fA")))     error_msg += "      fA\n";
    if(!(ch->GetBranch("fI")))     error_msg += "      fI\n";
    if(!(ch->GetBranch("fSa")))    error_msg += "      fSa\n";
    if(!(ch->GetBranch("fSx0")))   error_msg += "      fSx0\n";
    if(!(ch->GetBranch("fSx1")))   error_msg += "      fSx1\n";
    if(!(ch->GetBranch("fScbar"))) error_msg += "      fScbar\n";

    if(!(ch->GetBranch("fFastScintSpectrum"))) error_msg += "      fFastScintSpectrum\n";
    if(!(ch->GetBranch("fFastScintEnergies"))) error_msg += "      fFastScintEnergies\n";
    if(!(ch->GetBranch("fSlowScintSpectrum"))) error_msg += "      fSlowScintSpectrum\n";
    if(!(ch->GetBranch("fSlowScintEnergies"))) error_msg += "      fSlowScintEnergies\n";
    if(!(ch->GetBranch("fRIndexSpectrum")))    error_msg += "      fRIndexSpectrum\n";
    if(!(ch->GetBranch("fRIndexEnergies")))    error_msg += "      fRIndexEnergies\n";
    if(!(ch->GetBranch("fAbsLengthSpectrum"))) error_msg += "      fAbsLengthSpectrum\n";
    if(!(ch->GetBranch("fAbsLengthEnergies"))) error_msg += "      fAbsLengthEnergies\n";
    if(!(ch->GetBranch("fRayleighSpectrum")))  error_msg += "      fRayleighSpectrum\n";
    if(!(ch->GetBranch("fRayleighEnergies")))  error_msg += "      fRayleighEnergies\n";

    if(!(ch->GetBranch("fScintByParticleType"))) error_msg += "      fScintByParticleType\n";

    if(!(ch->GetBranch("fProtonScintYield")))        error_msg += "      fProtonScintYield\n";
    if(!(ch->GetBranch("fProtonScintYieldRatio")))   error_msg += "      fProtonScintYieldRatio\n";
    if(!(ch->GetBranch("fMuonScintYield")))          error_msg += "      fMuonScintYield\n";
    if(!(ch->GetBranch("fMuonScintYieldRatio")))     error_msg += "      fMuonScintYieldRatio\n";
    if(!(ch->GetBranch("fPionScintYield")))          error_msg += "      fPionScintYield\n";
    if(!(ch->GetBranch("fPionScintYieldRatio")))     error_msg += "      fPionScintYieldRatio\n";
    if(!(ch->GetBranch("fKaonScintYield")))          error_msg += "      fKaonScintYield\n";
    if(!(ch->GetBranch("fKaonScintYieldRatio")))     error_msg += "      fKaonScintYieldRatio\n";
    if(!(ch->GetBranch("fElectronScintYield")))      error_msg += "      fElectronScintYield\n";
    if(!(ch->GetBranch("fElectronScintYieldRatio"))) error_msg += "      fElectronScintYieldRatio\n";
    if(!(ch->GetBranch("fAlphaScintYield")))         error_msg += "      fAlphaScintYield\n";
    if(!(ch->GetBranch("fAlphaScintYieldRatio")))    error_msg += "      fAlphaScintYieldRatio\n";

    if(!(ch->GetBranch("fScintYield")))           error_msg += "      fScintYield\n";
    if(!(ch->GetBranch("fScintResolutionScale"))) error_msg += "      fScintResolutionScale\n";
    if(!(ch->GetBranch("fScintFastTimeConst")))   error_msg += "      fScintFastTimeConst\n";
    if(!(ch->GetBranch("fScintSlowTimeConst")))   error_msg += "      fScintSlowTimeConst\n";
    if(!(ch->GetBranch("fScintYieldRatio")))      error_msg += "      fScintYieldRatio\n";
    if(!(ch->GetBranch("fScintBirksConstant")))   error_msg += "      fScintBirksConstant\n";

    if(!(ch->GetBranch("fEnableCerenkovLight")))  error_msg += "      fEnableCerenkovLight\n";

    if(!(ch->GetBranch("fReflectiveSurfaceNames")))
      error_msg += "      fReflectiveSurfaceNames\n";
    if(!(ch->GetBranch("fReflectiveSurfaceEnergies")))
      error_msg += "      fReflectiveSurfaceEnergies\n";
    if(!(ch->GetBranch("fReflectiveSurfaceReflectances")))
      error_msg += "      fReflectiveSurfaceReflectances\n";
    if(!(ch->GetBranch("fReflectiveSurfaceDiffuseFractions")))
      error_msg += "      fReflectiveSurfaceDiffuseFractions\n";

    if(!error_msg.empty()) {

      throw LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return false;
    }

    std::vector<Double_t> *pEfield=nullptr;
    ch->SetBranchAddress("fEfield",&pEfield);

    ch->SetBranchAddress("fTemperature",&fTemperature);
    ch->SetBranchAddress("fElectronlifetime",&fElectronlifetime);
    ch->SetBranchAddress("fRadiationLength",&fRadiationLength);
    ch->SetBranchAddress("fArgon39DecayRate",&fArgon39DecayRate);

    ch->SetBranchAddress("fZ",&fZ);
    ch->SetBranchAddress("fA",&fA);
    ch->SetBranchAddress("fI",&fI);
    ch->SetBranchAddress("fSa",&fSa);
    ch->SetBranchAddress("fSk",&fSk);
    ch->SetBranchAddress("fSx0",&fSx0);
    ch->SetBranchAddress("fSx1",&fSx1);
    ch->SetBranchAddress("fScbar",&fScbar);

    // Optical parameters for LAr 
    std::vector<Double_t> *pFastScintSpectrum=nullptr;
    std::vector<Double_t> *pFastScintEnergies=nullptr;
    std::vector<Double_t> *pSlowScintSpectrum=nullptr;
    std::vector<Double_t> *pSlowScintEnergies=nullptr;
    std::vector<Double_t> *pRIndexSpectrum=nullptr;
    std::vector<Double_t> *pRIndexEnergies=nullptr;
    std::vector<Double_t> *pAbsLengthSpectrum=nullptr;
    std::vector<Double_t> *pAbsLengthEnergies=nullptr;
    std::vector<Double_t> *pRayleighSpectrum=nullptr;
    std::vector<Double_t> *pRayleighEnergies=nullptr;

    ch->SetBranchAddress("fFastScintSpectrum",&pFastScintSpectrum);
    ch->SetBranchAddress("fFastScintEnergies",&pFastScintEnergies);
    ch->SetBranchAddress("fSlowScintSpectrum",&pSlowScintSpectrum);
    ch->SetBranchAddress("fSlowScintEnergies",&pSlowScintEnergies);
    ch->SetBranchAddress("fRIndexSpectrum",&pRIndexSpectrum);
    ch->SetBranchAddress("fRIndexEnergies",&pRIndexEnergies);
    ch->SetBranchAddress("fAbsLengthSpectrum",&pAbsLengthSpectrum);
    ch->SetBranchAddress("fAbsLengthEnergies",&pAbsLengthEnergies);
    ch->SetBranchAddress("fRayleighSpectrum",&pRayleighSpectrum);
    ch->SetBranchAddress("fRayleighEnergies",&pRayleighEnergies);

    ch->SetBranchAddress("fScintByParticleType",&fScintByParticleType);

    ch->SetBranchAddress("fProtonScintYield", &fProtonScintYield);
    ch->SetBranchAddress("fProtonScintYieldRatio", &fProtonScintYieldRatio);
    ch->SetBranchAddress("fMuonScintYield", &fMuonScintYield);
    ch->SetBranchAddress("fMuonScintYieldRatio", &fMuonScintYieldRatio);
    ch->SetBranchAddress("fPionScintYield", &fPionScintYield);
    ch->SetBranchAddress("fPionScintYieldRatio", &fPionScintYieldRatio);
    ch->SetBranchAddress("fKaonScintYield", &fKaonScintYield);
    ch->SetBranchAddress("fKaonScintYieldRatio", &fKaonScintYieldRatio);
    ch->SetBranchAddress("fElectronScintYield", &fElectronScintYield);
    ch->SetBranchAddress("fElectronScintYieldRatio", &fElectronScintYieldRatio);
    ch->SetBranchAddress("fAlphaScintYield", &fAlphaScintYield);
    ch->SetBranchAddress("fAlphaScintYieldRatio", &fAlphaScintYieldRatio);
    
    ch->SetBranchAddress("fScintYield", &fScintYield);
    ch->SetBranchAddress("fScintResolutionScale", &fScintResolutionScale);
    ch->SetBranchAddress("fScintFastTimeConst", &fScintFastTimeConst);
    ch->SetBranchAddress("fScintSlowTimeConst", &fScintSlowTimeConst);
    ch->SetBranchAddress("fScintYieldRatio", &fScintYieldRatio);
    ch->SetBranchAddress("fScintBirksConstant", &fScintBirksConstant);
    
    ch->SetBranchAddress("fEnableCerenkovLight", &fEnableCerenkovLight);

    std::vector<std::string>            *pReflectiveSurfaceNames=nullptr;
    std::vector<Double_t>               *pReflectiveSurfaceEnergies=nullptr;
    std::vector<std::vector<Double_t> > *pReflectiveSurfaceReflectances=nullptr;
    std::vector<std::vector<Double_t> > *pReflectiveSurfaceDiffuseFractions=nullptr;

    ch->SetBranchAddress("fReflectiveSurfaceNames", &pReflectiveSurfaceNames);
    ch->SetBranchAddress("fReflectiveSurfaceEnergies", &pReflectiveSurfaceEnergies);
    ch->SetBranchAddress("fReflectiveSurfaceReflectances", &pReflectiveSurfaceReflectances);
    ch->SetBranchAddress("fReflectiveSurfaceDiffuseFractions", &pReflectiveSurfaceDiffuseFractions);

    ch->GetEntry(0);
    
    // Copy vector contents

    for(size_t i=0; i<pEfield->size(); ++i)
      fEfield.push_back(pEfield->at(i));

    size_t n_entries = pFastScintSpectrum->size();
    fFastScintSpectrum.reserve(n_entries);
    fFastScintEnergies.reserve(n_entries);
    for(size_t i=0; i<n_entries; ++i) {
      fFastScintSpectrum.push_back(pFastScintSpectrum->at(i));
      fFastScintEnergies.push_back(pFastScintEnergies->at(i));
    }
    n_entries = pSlowScintSpectrum->size();
    fSlowScintSpectrum.reserve(n_entries);
    fSlowScintEnergies.reserve(n_entries);
    for(size_t i=0; i<n_entries; ++i) {
      fSlowScintSpectrum.push_back(pSlowScintSpectrum->at(i));
      fSlowScintEnergies.push_back(pSlowScintEnergies->at(i));
    }
    n_entries = pRIndexSpectrum->size();
    fRIndexSpectrum.reserve(n_entries);
    fRIndexEnergies.reserve(n_entries);
    for(size_t i=0; i<n_entries; ++i) {
      fRIndexSpectrum.push_back(pRIndexSpectrum->at(i));
      fRIndexEnergies.push_back(pRIndexEnergies->at(i));
    }
    n_entries = pAbsLengthSpectrum->size();
    fAbsLengthSpectrum.reserve(n_entries);
    fAbsLengthEnergies.reserve(n_entries);
    for(size_t i=0; i<n_entries; ++i) {
      fAbsLengthSpectrum.push_back(pAbsLengthSpectrum->at(i));
      fAbsLengthEnergies.push_back(pAbsLengthEnergies->at(i));
    }
    n_entries = pRayleighSpectrum->size();
    fRayleighSpectrum.reserve(n_entries);
    fRayleighEnergies.reserve(n_entries);
    for(size_t i=0; i<n_entries; ++i) {
      fRayleighSpectrum.push_back(pRayleighSpectrum->at(i));
      fRayleighEnergies.push_back(pRayleighEnergies->at(i));
    }


    size_t n_surface = pReflectiveSurfaceNames->size();
    fReflectiveSurfaceNames.reserve(n_surface);
    fReflectiveSurfaceEnergies.reserve(n_surface);
    fReflectiveSurfaceReflectances.reserve(n_surface);
    fReflectiveSurfaceDiffuseFractions.reserve(n_surface);
    for(size_t i=0; i<n_surface; ++i) {

      fReflectiveSurfaceNames.push_back(pReflectiveSurfaceNames->at(i));
      fReflectiveSurfaceEnergies.push_back(pReflectiveSurfaceEnergies->at(i));
      fReflectiveSurfaceReflectances.push_back(pReflectiveSurfaceReflectances->at(i));
      fReflectiveSurfaceDiffuseFractions.push_back(pReflectiveSurfaceDiffuseFractions->at(i));

    }
    
    delete ch;
    return true;
  }

  Double_t LArProperties::Density(Double_t temperature) const
  {
    // Default temperature use internal value.
    if(temperature == 0.)
      temperature = Temperature();
    
    Double_t density = -0.00615*temperature + 1.928;
    
    return density;
  }

  Double_t LArProperties::Efield(UInt_t planegap) const
  {
    if(planegap >= fEfield.size())
      throw LArUtilException("requesting Electric field in a plane gap that is not defined");
    
    return fEfield.at(planegap);
  }


  //------------------------------------------------------------------------------------//
  Double_t LArProperties::DriftVelocity(Double_t efield, Double_t temperature) const {
    
    // Drift Velocity as a function of Electric Field and LAr Temperature
    // from : W. Walkowiak, NIM A 449 (2000) 288-294
    //
    // Efield should have units of kV/cm
    // Temperature should have units of Kelvin
    
    // Default Efield, use internal value.
    if(efield == 0. && temperature == 0.)
      //  efield = Efield();
      return Vd();
    //
    if(efield > 4.0) {

      std::ostringstream msg;
      msg <<"DriftVelocity Warning! : E-field value of "
	  << efield
	  << " kV/cm is outside of range covered by drift"
	  << " velocity parameterization. Returned value"
	  << " may not be correct";
      print(larlite::msg::kWARNING,__FUNCTION__,msg.str());
    }    
    
    // Default temperature use internal value.
    //if(temperature == 0.)
    //temperature = Temperature();
    
    if(temperature < 87.0 || temperature > 94.0) {
      std::ostringstream msg;
      msg << "DriftVelocity Warning! : Temperature value of "
	  << temperature
	  << " K is outside of range covered by drift velocity"
	  << " parameterization. Returned value may not be"
	  << " correct";
      print(larlite::msg::kWARNING,__FUNCTION__,msg.str());
    }    
    
    Double_t tshift = -87.203+temperature;
    Double_t xFit = 0.0938163-0.0052563*tshift-0.0001470*tshift*tshift;
    Double_t uFit = 5.18406+0.01448*tshift-0.003497*tshift*tshift-0.000516*tshift*tshift*tshift;
    Double_t vd;
    
    
    // Icarus Parameter Set, use as default
    Double_t  P1 = -0.04640; // K^-1
    Double_t  P2 = 0.01712;  // K^-1
    Double_t  P3 = 1.88125;   // (kV/cm)^-1
    Double_t  P4 =  0.99408;    // kV/cm
    Double_t  P5 =  0.01172;   // (kV/cm)^-P6
    Double_t  P6 =  4.20214;
    Double_t  T0 =  105.749;  // K
    // Walkowiak Parameter Set
    Double_t    P1W = -0.01481; // K^-1
    Double_t  P2W = -0.0075;  // K^-1
    Double_t   P3W =  0.141;   // (kV/cm)^-1
    Double_t   P4W =  12.4;    // kV/cm
    Double_t   P5W =  1.627;   // (kV/cm)^-P6
    Double_t   P6W =  0.317;
    Double_t   T0W =  90.371;  // K
    
    // From Craig Thorne . . . currently not documented
    // smooth transition from linear at small fields to 
    //     icarus fit at most fields to Walkowiak at very high fields
    if (efield < xFit) vd=efield*uFit;
    else if (efield<0.619) { 
      vd = ((P1*(temperature-T0)+1)
	    *(P3*efield*std::log(1+P4/efield) + P5*std::pow(efield,P6))
	    +P2*(temperature-T0));
    }
    else if (efield<0.699) {
      vd = 12.5*(efield-0.619)*((P1W*(temperature-T0W)+1)
				*(P3W*efield*std::log(1+P4W/efield) + P5W*std::pow(efield,P6W))
				+P2W*(temperature-T0W))+
	12.5*(0.699-efield)*((P1*(temperature-T0)+1)
			     *(P3*efield*std::log(1+P4/efield) + P5*std::pow(efield,P6))
			     +P2*(temperature-T0));
    }
    else {
      vd = ((P1W*(temperature-T0W)+1)
	    *(P3W*efield*std::log(1+P4W/efield) + P5W*std::pow(efield,P6W))
	    +P2W*(temperature-T0W));     
    }
    
    vd /= 10.;

    return vd; // in cm/us
  }

  //----------------------------------------------------------------------------------
  // The below function assumes that the user has applied the lifetime correction and
  // effective pitch between the wires (usually after 3D reconstruction). Using with
  // mean wire pitch will not give correct results.
  // parameters:
  //  dQdX in electrons/cm, charge (amplitude or integral obtained) divided by effective pitch for a given 3D track.
  // returns dEdX in MeV/cm
  Double_t LArProperties::BirksCorrection(Double_t dQdx) const
  {
    // Correction for charge quenching using parameterization from
    // S.Amoruso et al., NIM A 523 (2004) 275
    
    Double_t  A3t    = kRecombA;
    Double_t  K3t    = kRecombk;                     // in KV/cm*(g/cm^2)/MeV
    Double_t  rho    = this->Density();                    // LAr density in g/cm^3
    Double_t Wion    = 1000./kGeVToElectrons;        // 23.6 eV = 1e, Wion in MeV/e
    Double_t Efield  = this->Efield();                     // Electric Field in the drift region in KV/cm
    K3t           /= rho;                                // KV/MeV
    Double_t dEdx    = dQdx/(A3t/Wion-K3t/Efield*dQdx);    //MeV/cm
    
    return dEdx;
  }

  Double_t LArProperties::BirksInverse(Double_t dEdx) const
  {
    // Correction for charge quenching using parameterization from
    // S.Amoruso et al., NIM A 523 (2004) 275
    
    Double_t  A3t    = kRecombA;
    Double_t  K3t    = kRecombk;                     // in KV/cm*(g/cm^2)/MeV
    Double_t  rho    = this->Density();                    // LAr density in g/cm^3
    Double_t Wion    = 1000./kGeVToElectrons;        // 23.6 eV = 1e, Wion in MeV/e
    Double_t Efield  = this->Efield();                     // Electric Field in the drift region in KV/cm
    K3t           /= rho;                                // KV/MeV

    Double_t dQdx = (A3t/Wion) / ( K3t / Efield * dEdx + 1); 

    return dQdx;
  }
  
  // Modified Box model correction 
  Double_t LArProperties::ModBoxCorrection(Double_t dQdx) const
  {
    // Modified Box model correction has better behavior than the Birks
    // correction at high values of dQ/dx.
    Double_t  rho    = this->Density();                    // LAr density in g/cm^3
    Double_t Wion    = 1000./kGeVToElectrons;        // 23.6 eV = 1e, Wion in MeV/e
    Double_t Efield  = this->Efield();                     // Electric Field in the drift region in KV/cm
    Double_t Beta    = kModBoxB / (rho * Efield);
    Double_t Alpha   = kModBoxA;
    Double_t dEdx = (exp(Beta * Wion * dQdx ) - Alpha) / Beta;
    
    return dEdx;
  }

  // Modified Box model correction 
  Double_t LArProperties::ModBoxInverse(Double_t dEdx) const
  {
    // Modified Box model correction has better behavior than the Birks
    // correction at high values of dQ/dx.
    Double_t  rho    = this->Density();                    // LAr density in g/cm^3
    Double_t Wion    = 1000./kGeVToElectrons;        // 23.6 eV = 1e, Wion in MeV/e
    Double_t Efield  = this->Efield();                     // Electric Field in the drift region in KV/cm
    Double_t Beta    = kModBoxB / (rho * Efield);
    Double_t Alpha   = kModBoxA;
    
    Double_t dQdx = log ( Alpha + Beta * dEdx ) / ( Beta * Wion );
    
    return dQdx;
  }

  //----------------------------------------------------------------------------------
  // Restricted mean energy loss (dE/dx) in units of MeV/cm.
  //
  // For unrestricted mean energy loss, set tcut = 0, or tcut large.
  //
  // Arguments:
  //
  // mom  - Momentum of incident particle in GeV/c.
  // mass - Mass of incident particle in GeV/c^2.
  // tcut - Maximum kinetic energy of delta rays (MeV).
  //
  // Returned value is positive.
  //
  // Based on Bethe-Bloch formula as contained in particle data book.
  // Material parameters (stored in larproperties.fcl) are taken from
  // pdg web site http://pdg.lbl.gov/AtomicNuclearProperties/
  //
  Double_t LArProperties::Eloss(Double_t mom, Double_t mass, Double_t tcut) const
  {
    // Some constants.
    
    Double_t K = 0.307075;     // 4 pi N_A r_e^2 m_e c^2 (MeV cm^2/mol).
    Double_t me = 0.510998918; // Electron mass (MeV/c^2).
    
    // Calculate kinematic quantities.
    
    Double_t bg = mom / mass;           // beta*gamma.
    Double_t gamma = sqrt(1. + bg*bg);  // gamma.
    Double_t beta = bg / gamma;         // beta (velocity).
    Double_t mer = 0.001 * me / mass;   // electron mass / mass of incident particle.
    Double_t tmax = 2.*me* bg*bg / (1. + 2.*gamma*mer + mer*mer);  // Maximum delta ray energy (MeV).
    
    // Make sure tcut does not exceed tmax.
    
    if(tcut == 0. || tcut > tmax)
      tcut = tmax;
    
    // Calculate density effect correction (delta).
    
    Double_t x = std::log10(bg);
    Double_t delta = 0.;
    if(x >= fSx0) {
      delta = 2. * std::log(10.) * x - fScbar;
      if(x < fSx1)
	delta += fSa * std::pow(fSx1 - x, fSk);
    }
    
    // Calculate stopping number.
    
    Double_t B = 0.5 * std::log(2.*me*bg*bg*tcut / (1.e-12 * fI*fI))
      - 0.5 * beta*beta * (1. + tcut / tmax) - 0.5 * delta;
    
    // Don't let the stopping number become negative.
    
    if(B < 1.)
      B = 1.;
    
    // Calculate dE/dx.
    
    Double_t dedx = Density() * K*fZ*B / (fA * beta*beta);
    
    // Done.
    
    return dedx;
  }
  
  //----------------------------------------------------------------------------------
  // Energy loss fluctuation (sigma_E^2 / length in MeV^2/cm).
  //
  // Arguments:
  //
  // mom  - Momentum of incident particle in GeV/c.
  //
  // Based on Bichsel formula referred to but not given in pdg.
  //
  Double_t LArProperties::ElossVar(Double_t mom, Double_t mass) const
  {
    // Some constants.
    
    Double_t K = 0.307075;     // 4 pi N_A r_e^2 m_e c^2 (MeV cm^2/mol).
    Double_t me = 0.510998918; // Electron mass (MeV/c^2).
    
    // Calculate kinematic quantities.
    
    Double_t bg = mom / mass;          // beta*gamma.
    Double_t gamma2 = 1. + bg*bg;      // gamma^2.
    Double_t beta2 = bg*bg / gamma2;   // beta^2.
    
    // Calculate final result.
    
    Double_t result = gamma2 * (1. - 0.5 * beta2) * me * (fZ / fA) * K * Density();
    return result;
  }

  //---------------------------------------------------------------------------------
  std::map<Double_t,Double_t> LArProperties::FastScintSpectrum() const
  {
    if(fFastScintSpectrum.size()!=fFastScintEnergies.size()){
      std::ostringstream msg;
      msg << "The vectors specifying the fast scintillation spectrum are "
	  << " different sizes - " << fFastScintSpectrum.size()
	  << " " << fFastScintEnergies.size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fFastScintSpectrum.size(); ++i)
      ToReturn[fFastScintEnergies.at(i)]=fFastScintSpectrum.at(i);
    
    return ToReturn;
  }

  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::SlowScintSpectrum() const
  {
    if(fSlowScintSpectrum.size()!=fSlowScintEnergies.size()){
      std::ostringstream msg;
      msg << "The vectors specifying the slow scintillation spectrum are "
	  << " different sizes - " << fFastScintSpectrum.size()
	  << " " << fFastScintEnergies.size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fSlowScintSpectrum.size(); ++i)
      ToReturn[fSlowScintEnergies.at(i)]=fSlowScintSpectrum.at(i);
    
    return ToReturn;
  }
  
  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::RIndexSpectrum() const
  {
    if(fRIndexSpectrum.size()!=fRIndexEnergies.size()){
      std::ostringstream msg;
      msg << "The vectors specifying the RIndex spectrum are "
	  << " different sizes - " << fRIndexSpectrum.size()
	  << " " << fRIndexEnergies.size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fRIndexSpectrum.size(); ++i)
      ToReturn[fRIndexEnergies.at(i)]=fRIndexSpectrum.at(i);
    
    return ToReturn;
  }


  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::AbsLengthSpectrum() const
  {
    if(fAbsLengthSpectrum.size()!=fAbsLengthEnergies.size()){
      std::ostringstream msg;
      msg << "The vectors specifying the Abs Length spectrum are "
	  << " different sizes - " << fAbsLengthSpectrum.size()
	  << " " << fAbsLengthEnergies.size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fAbsLengthSpectrum.size(); ++i)
      ToReturn[fAbsLengthEnergies.at(i)]=fAbsLengthSpectrum.at(i);
    
    return ToReturn;
  }

  //---------------------------------------------------------------------------------
  std::map<Double_t, Double_t> LArProperties::RayleighSpectrum() const
  {
    if(fRayleighSpectrum.size()!=fRayleighEnergies.size()){
      std::ostringstream msg;
      msg << "The vectors specifying the rayleigh spectrum are "
	  << " different sizes - " << fRayleighSpectrum.size()
	  << " " << fRayleighEnergies.size();
      throw LArUtilException(msg.str());
    }
    
    std::map<Double_t, Double_t> ToReturn;
    for(size_t i=0; i!=fRayleighSpectrum.size(); ++i)
      ToReturn[fRayleighEnergies.at(i)]=fRayleighSpectrum.at(i);
    
    return ToReturn;
  }

  //---------------------------------------------------------------------------------
  std::map<std::string, std::map<Double_t,Double_t> > LArProperties::SurfaceReflectances() const
  {
    std::map<std::string, std::map<Double_t, Double_t> > ToReturn;
    
    if(fReflectiveSurfaceNames.size()!=fReflectiveSurfaceReflectances.size()){
      std::ostringstream msg;
	msg << "The vectors specifying the surface reflectivities "
	    << "do not have consistent sizes";
	throw LArUtilException(msg.str());
    }
    for(size_t i=0; i!=fReflectiveSurfaceNames.size(); ++i){
      if(fReflectiveSurfaceEnergies.size()!=fReflectiveSurfaceReflectances.at(i).size()){
	std::ostringstream msg;
	msg << "The vectors specifying the surface reflectivities do not have consistent sizes";
	throw LArUtilException(msg.str());
      }
    }
    for(size_t iName=0; iName!=fReflectiveSurfaceNames.size(); ++iName)
      for(size_t iEnergy=0; iEnergy!=fReflectiveSurfaceEnergies.size(); ++iEnergy)
	ToReturn[fReflectiveSurfaceNames.at(iName)][fReflectiveSurfaceEnergies.at(iEnergy)]=fReflectiveSurfaceReflectances.at(iName)[iEnergy];
    
    return ToReturn;
    
  }
  
  //---------------------------------------------------------------------------------
  std::map<std::string, std::map<Double_t,Double_t> > LArProperties::SurfaceReflectanceDiffuseFractions() const
  {
    std::map<std::string, std::map<Double_t, Double_t> > ToReturn;
    
    if(fReflectiveSurfaceNames.size()!=fReflectiveSurfaceDiffuseFractions.size()){
      std::ostringstream msg;
      msg << "The vectors specifying the surface reflectivities do not have consistent sizes";
      LArUtilException(msg.str());
    }
    for(size_t i=0; i!=fReflectiveSurfaceNames.size(); ++i){
      if(fReflectiveSurfaceEnergies.size()!=fReflectiveSurfaceDiffuseFractions.at(i).size()){
	std::ostringstream msg;
	msg << "The vectors specifying the surface reflectivities do not have consistent sizes";
	throw LArUtilException(msg.str());
      }
    }
    for(size_t iName=0; iName!=fReflectiveSurfaceNames.size(); ++iName)
      for(size_t iEnergy=0; iEnergy!=fReflectiveSurfaceEnergies.size(); ++iEnergy)
	ToReturn[fReflectiveSurfaceNames.at(iName)][fReflectiveSurfaceEnergies.at(iEnergy)]=fReflectiveSurfaceDiffuseFractions.at(iName)[iEnergy];
    
    return ToReturn;
  }
  
}
#endif
