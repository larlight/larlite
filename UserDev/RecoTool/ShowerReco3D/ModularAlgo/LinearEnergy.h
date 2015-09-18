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

  class LinearEnergy : ShowerRecoModuleBase{

  public:

    /// Default constructor
    LinearEnergy();

    /// Default destructor
    ~LinearEnergy(){}

    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

    void SetUseArea(bool on) { _useArea = on; }

    void SetUseModBox(bool on) { _useModBox = on; }
    
    void SetFillTree(bool on) { _fill_tree = on; }

    void initialize();

  private:

    /// Calorimetry algorithm
    ::calo::CalorimetryAlg _caloAlg;

    /// flag for whether to decide if we should use the hit area or amplitude for energy calculations
    bool _useArea;

    /// flag to decide if to use the ModBox or Birks models
    bool _useModBox;
    
    /// boolean on whether to fill tree or not
    bool _fill_tree;

    /// calorimetry + other conversion factors
    double _fC_to_e;
    double _e_to_eV;
    double _eV_to_MeV;
    double _ADC_to_mV;
    double _shp_time;
    double _asic_gain;
    double _energy_conversion;
    double _charge_conversion;
    double _tau; // electron lifetime in usec
    double _timetick; // sampling size in usec
    double _recomb_factor; // factor by which charge should be suppresse0d due to recombination effects...

    // tree variables
    std::vector<double> _dE_v;
    std::vector<double> _dEdx_v;
    std::vector<double> _dQ_v;
    int _pl;

  };
  
} // showerreco

#endif
/** @} */ // end of doxygen group 
