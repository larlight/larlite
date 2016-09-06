/**
 * \file LinearEnergy.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class LinearEnergy
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef LINEARENERGY_H
#define LINEARENERGY_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
#include "AnalysisAlg/CalorimetryAlg.h"

/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

class LinearEnergy : ShowerRecoModuleBase {

public:

    /// Default constructor
    LinearEnergy();

    /// Default destructor
    ~LinearEnergy() {}

    void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);

    void SetUseArea(bool on) { _useArea = on; }

    void SetUseModBox(bool on) { _useModBox = on; }

    void SetFillTree(bool on) { _fill_tree = on; }

    /// set electron lifetime [ms]
    void SetElectronLifetime(double t) { _tau = t; }

    /// set recombination factor
    void SetRecombFactor(double r) { _recomb_factor = r; }

    /// use custom calibration constants [ ADC to fC ]
    void SetCalibrationConst(double c) { _ADC_to_fC = c; CalculateEnergyConversion(); _custom_calib = true; }

    void initialize();

    // set plane-dependent gains
    void SetHitRecoCorrection(double f, int pl)
    {
      if ( pl >= _HitRecoCorrection_v.size() )
	{ std::cout << "Cannot set gain @ this plane..." << std::endl; return; }
      _HitRecoCorrection_v[pl] = f;
      return;
    }

    void SetClusteringCorrection(double f, int pl)
    {
      if ( pl >= _HitRecoCorrection_v.size() )
	{ std::cout << "Cannot set gain @ this plane..." << std::endl; return; }
      _ClusteringCorrection_v[pl] = f;
      return;
    }
    
 private:
    
    /// Calorimetry algorithm
    ::calo::CalorimetryAlg _caloAlg;

    /// flag for whether to decide if we should use the hit area or amplitude for energy calculations
    bool _useArea;

    /// flag to decide if to use the ModBox or Birks models
    bool _useModBox;
    /// use custom calibration constants
    bool _custom_calib;

    /// boolean on whether to fill tree or not
    bool _fill_tree;

    /// calorimetry + other conversion factors
    double _fC_to_e;
    double _e_to_eV;
    double _eV_to_MeV;
    double _ADC_to_mV;
    double _shp_time;
    double _asic_gain;
    double _ADC_to_MeV;
    double _ADC_to_fC;
    double _tau; // electron lifetime in usec
    double _timetick; // sampling size in usec
    double _recomb_factor; // factor by which charge should be suppresse0d due to recombination effects...

    // tree variables
    std::vector<double> _dE_v;
    std::vector<double> _dEdx_v;
    std::vector<double> _dQ_v;
    int _pl;
    std::vector<double> _tick_v;

    // per-plane shower reco calibrations
    // calibration for hit reconstruction efficiency.
    // calculated by summing all the charge in the hits in an event (for single particle showers)
    // and seeing how much charge is missing to get the "true" shower energy
    std::vector<double> _HitRecoCorrection_v;
    // calibration for clustering efficiency
    // this is clustering-algirhm specific. how much charge misses, on average, for a cluster?
    // account for this factor using this calibration constant
    std::vector<double> _ClusteringCorrection_v;


    // calculate ADC (per-hit) to MeV conversion factor
    void CalculateEnergyConversion() { _ADC_to_MeV = _ADC_to_fC * _fC_to_e * _e_to_eV * _eV_to_MeV; }

};

} // showerreco

#endif
/** @} */ // end of doxygen group
