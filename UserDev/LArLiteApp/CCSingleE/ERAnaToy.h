/**
 * \file ERAnaToy.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaToy
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANATOY_H
#define ERTOOL_ERANATOY_H

#include "ERTool/Base/AnaBase.h"
#include <TH1D.h>
namespace ertool {

  /**
     \class ERAnaToy
     User custom Analysis class made by kazuhiro
   */
  class ERAnaToy : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaToy();

    /// Default destructor
    virtual ~ERAnaToy(){};

    /// Reset function
    virtual void Reset();

    virtual void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    virtual void ProcessEnd(TFile* fout);

    bool _verbose;

  protected:

    TH1D* hShowerCount;
    Particle _target_particle;
  };
}
#endif

/** @} */ // end of doxygen group 
