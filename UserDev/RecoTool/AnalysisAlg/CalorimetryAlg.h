////////////////////////////////////////////////////////////////////////
// \file CalorimetryAlg.h
//
// \brief Functions to calculate dE/dx. Based on code in Calorimetry.cxx
//
// \author andrzej.szelc@yale.edu
//
////////////////////////////////////////////////////////////////////////
#ifndef RECOTOOL_UTIL_CALORIMETRYALG_H
#define RECOTOOL_UTIL_CALORIMETRYALG_H

//#include "fhiclcpp/ParameterSet.h"

//#include "Utilities/LArProperties.h"
//#include "Geometry/Geometry.h"
//#include "Utilities/DetectorProperties.h"
#include <vector>

#include "LArUtil/LArProperties.h"
#include "LArUtil/DetectorProperties.h"
#include "LArUtil/PxUtils.h"
#include "AnalysisAlgConstants.h"

///General LArSoft Utilities
namespace calo{

  class CalorimetryAlg {
    
  public:
    
    CalorimetryAlg();
    //CalorimetryAlg(fhicl::ParameterSet const& pset);
    
    ~CalorimetryAlg();
    
    //void   reconfigure(fhicl::ParameterSet const& pset);
    
    //double dEdx_AMP(art::Ptr< recob::Hit >  hit, double pitch) const;
    //double dEdx_AMP(recob::Hit const&  hit, double pitch) const;
    double dEdx_AMP(const larutil::PxHit *hit, double pitch) const;
    double dEdx_AMP(double dQ, double time, double pitch, unsigned int plane) const;
    double dEdx_AMP(double dQdx,double time, unsigned int plane) const;
    
    //double dEdx_AREA(art::Ptr< recob::Hit >  hit, double pitch) const;
    //double dEdx_AREA(recob::Hit const&  hit, double pitch) const;
    double dEdx_AREA(const larutil::PxHit *hit, double pitch) const;
    double dEdx_AREA(double dQ,double time, double pitch, unsigned int plane) const;
    double dEdx_AREA(double dQdx,double time, unsigned int plane) const;
    
    double LifetimeCorrection(double time) const;

    double ElectronsFromADCPeak(double adc, unsigned short plane) const
    { return adc / fCalAmpConstants[plane]; }

    double ElectronsFromADCArea(double area, unsigned short plane) const
    { return area / fCalAreaConstants[plane]; }

    void setUseModBox(bool on) { fUseModBox = on; }

  private:
    
    //art::ServiceHandle<geo::Geometry> geom; 
    //art::ServiceHandle<util::LArProperties> LArProp;
    //art::ServiceHandle<util::DetectorProperties> detprop;

    ::larutil::LArProperties* LArProp;
    ::larutil::DetectorProperties* detprop;
    
    double dEdx_from_dQdx_e(double dQdx_e,double time) const;
    
    std::vector< double > fCalAmpConstants;
    std::vector< double > fCalAreaConstants;
    bool fUseModBox;

    double _tau;      // electron lifetime in usec
    double _timetick; // time sample in usec
    
  }; // class CalorimetryAlg
} //namespace calo
#endif // UTIL_CALORIMETRYALG_H
