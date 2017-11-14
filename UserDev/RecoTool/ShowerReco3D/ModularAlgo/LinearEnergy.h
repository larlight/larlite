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

#include "LArUtil/Geometry.h"

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

    /// 3D charge correction function
    double ChargeCorrection(const double& q, const double& w, const double& t, const TVector3& dir, const TVector3& vtx);

    // create a map for calorimetric corrections for position-dependent response variations
    void CreateResponseMap(const double& stepsize);

    // set response map value
    void SetResponse(const double& x, const double& y, const double& z, const double& q);

    /// find nearest cell response value (by averaging)
    double NearestCell(const size_t& i, const size_t& j, const size_t& k);

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
    double _dEdx;
    double _dQ;
    double _lifetime_corr;
    double _electrons;
    double _tick;

    // position-dependent response map
    std::vector< std::vector< std::vector< double > > >_responseMap;
    double _responseStep;


};

} // showerreco

#endif
/** @} */ // end of doxygen group
