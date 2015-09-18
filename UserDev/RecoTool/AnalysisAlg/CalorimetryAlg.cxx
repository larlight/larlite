////////////////////////////////////////////////////////////////////////
//  \file CalorimetryAlg.cxx
//
//  \brief Functions to calculate dE/dx. Based on code in Calorimetry.cxx
//
// andrzej.szelc@yale.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef RECOTOOL_CALORIMETRYALG_CXX
#define RECOTOOL_CALORIMETRYALG_CXX


#include "TMath.h"

#include "LArUtil/Geometry.h"

#include "CalorimetryAlg.h"

namespace calo{

  CalorimetryAlg::CalorimetryAlg() : LArProp(nullptr), 
				     detprop(nullptr),
				     fCalAmpConstants(),
				     fCalAreaConstants(),
				     fUseModBox(false)
  {
    
    detprop = (::larutil::DetectorProperties*)(::larutil::DetectorProperties::GetME());
    LArProp = (::larutil::LArProperties*)(::larutil::LArProperties::GetME());

    _tau = LArProp->ElectronLifetime();
    _timetick = detprop->SamplingRate()*1.e-3;    //time sample in microsec
    
    fCalAmpConstants.clear();
    fCalAreaConstants.clear();

    auto geo = ::larutil::Geometry::GetME();

    switch(::larutil::LArUtilConfig::Detector()) {
      
    case ::larlite::geo::kArgoNeuT:
      for(size_t i=0; i < geo->Nplanes(); ++i) {
	fCalAmpConstants.push_back  ( kCalAmpConstants_ARGONEUT[i]  );
	fCalAreaConstants.push_back ( kCalAreaConstants_ARGONEUT[i] );
      }
      break;
    case ::larlite::geo::kMicroBooNE:
      for(size_t i=0; i < geo->Nplanes(); ++i) {
	fCalAmpConstants.push_back  ( kCalAmpConstants_UB[i]  );
	fCalAreaConstants.push_back ( kCalAreaConstants_UB[i] );
      }
      break;
    default:
      std::cerr << "\033[93mDetector ID not supported:"
		<< ::larutil::LArUtilConfig::Detector()
		<< "\033[00m" 
		<< std::endl;
      throw std::exception();
    }

  }

  //--------------------------------------------------------------------
  CalorimetryAlg::~CalorimetryAlg() 
  {
    
  }
  
  // ----------------------------------------------------------------------------------//
  
  double CalorimetryAlg::dEdx_AMP(const larutil::PxHit *hit, double pitch) const
  {
    return dEdx_AMP(hit->peak/pitch, hit->t, hit->plane);
  }
  
  
  
  ///\todo The plane argument should really be for a view instead
  // ----------------------------------------------------------------------------------//
  double CalorimetryAlg::dEdx_AMP(double dQ, double time, double pitch, unsigned int plane) const
  {
    double dQdx   = dQ/pitch;           // in ADC/cm
    return dEdx_AMP(dQdx, time, plane);
  }

  // ----------------------------------------------------------------------------------//
  double CalorimetryAlg::dEdx_AMP(double dQdx,double time, unsigned int plane) const
  {
    double dQdx_e = ElectronsFromADCPeak(dQdx,plane);  // Conversion from ADC/cm to e/cm
    return dEdx_from_dQdx_e(dQdx_e,time);
  }
  

  double CalorimetryAlg::dEdx_AREA(const larutil::PxHit *hit, double pitch) const
  {
    return dEdx_AREA(hit->charge/pitch, hit->t, hit->plane);
  }
  
  
  // ----------------------------------------------------------------------------------//
  double CalorimetryAlg::dEdx_AREA(double dQ,double time, double pitch, unsigned int plane) const
  {
    double dQdx   = dQ/pitch;           // in ADC/cm
    return dEdx_AREA(dQdx, time, plane);
  }
  
  // ----------------------------------------------------------------------------------//  
  double CalorimetryAlg::dEdx_AREA(double dQdx,double time, unsigned int plane) const
  {
    double dQdx_e = ElectronsFromADCArea(dQdx,plane);  // Conversion from ADC/cm to e/cm
    return dEdx_from_dQdx_e(dQdx_e, time);
  }
    
  // ----------------- apply Lifetime and recombination correction.  -----------------//
  double CalorimetryAlg::dEdx_from_dQdx_e(double dQdx_e, double time) const
  {
    double t_corr = LifetimeCorrection(time);   // Lifetime Correction (dQdx_e in e/cm)
    dQdx_e *= t_corr;
    if(fUseModBox) {
      return LArProp->ModBoxCorrection(dQdx_e);
    } else {
      return LArProp->BirksCorrection(dQdx_e);
    }
  }
  
  // -------------------------------------------------------------//  
  double calo::CalorimetryAlg::LifetimeCorrection(double time) const
  {  
    float t = time;
    time = t * _timetick;  //  (in microsec)
    double correction = exp(time/_tau);
    return correction;
  }

} // namespace

#endif
