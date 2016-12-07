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

    void SetFillTree(bool on) { _fill_tree = on; }

    /// set electron lifetime [ms]
    void SetElectronLifetime(double t) { _tau = t; }

    /// set recombination factor
    void SetRecombFactor(double r) { _recomb_factor = r; }

    /// set ADC -> e- calibration constant
    void SetElecGain(double q) { _elec_gain = q; }

    void initialize();

 private:
    
    /// boolean on whether to fill tree or not
    bool _fill_tree;
    
    // calbration gain
    double _elec_gain;

    // recombination factor
    double _recomb_factor; // e- / ADC

    /// calorimetry + other conversion factors
    double _fC_to_e;
    double _e_to_eV;
    double _eV_to_MeV;
    double _ADC_to_mV;
    double _ADC_to_MeV;
    double _ADC_to_fC;
    double _clocktick;
    double _tau; // electron lifetime in usec

    // tree variables
    double _dE;
    double _dQ;
    double _lifetime_corr;
    double _electrons;
    double _tick;

};

} // showerreco

#endif
/** @} */ // end of doxygen group
