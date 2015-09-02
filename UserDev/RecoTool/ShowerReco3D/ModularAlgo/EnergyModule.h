/**
 * \file EnergyModule.h
 *
 * \ingroup ModularAlgo
 * 
 * \brief Class def header for a class EnergyModule
 *
 * @author david caratelli
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef ENERGYMODULE_H
#define ENERGYMODULE_H

#include <iostream>
#include "ShowerRecoModuleBase.h"
#include "AnalysisAlg/CalorimetryAlg.h"

/**
   \class ShowerRecoModuleBase
   User defined class ShowerRecoModuleBase ... these comments are used to generate
   doxygen documentation!
 */
namespace showerreco {

  class EnergyModule : ShowerRecoModuleBase{

  public:

    /// Default constructor
    EnergyModule();

    /// Default destructor
    ~EnergyModule(){}

    void do_reconstruction(const ShowerClusterSet_t &, Shower_t &);

    void SetUseArea(bool on) { _useArea = on; }

    void SetUseBirks(bool on=true) { _caloAlg->setUseModBox(!on); }

    void SetUseBox(bool on = true) { _caloAlg->setUseModBox(on); }

  private:

    /// Calorimetry algorithm
    ::calo::CalorimetryAlg *_caloAlg;

    /// flag for whether to decide if we should use the hit area or amplitude for energy calculations
    bool _useArea;

    /// flag to decide which ionization module to use
    bool _useBirks;

  };
  
} // showerreco

#endif
/** @} */ // end of doxygen group 
