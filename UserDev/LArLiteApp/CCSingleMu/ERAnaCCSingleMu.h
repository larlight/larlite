/**
 * \file ERAnaCCSingleMu.h
 *
 * \ingroup CCSingleMu
 * 
 * \brief Class def header for a class ERAnaCCSingleMu
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleMu

    @{*/

#ifndef ERTOOL_ERANACCSINGLEMU_H
#define ERTOOL_ERANACCSINGLEMU_H

#include "ERTool/Base/AnaBase.h"
#include <TH1D.h>
namespace ertool {

  /**
     \class ERAnaCCSingleMu
     User custom Analysis class made by kazuhiro
   */
  class ERAnaCCSingleMu : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaCCSingleMu();

    /// Default destructor
    virtual ~ERAnaCCSingleMu(){}

    /// Reset function
    virtual void Reset();

    /// Called @ before processing the first event sample
    virtual void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    bool AnalyzeEventData(const EventData &data, const ParticleSet& ps);

    /// Called after processing the last event sample
    virtual void ProcessEnd(TFile* fout=nullptr);

  private:
    bool   _use_mc;
    TH1D* hMCPrimaryCtr;
    TH1D* hMCPrimaryPID;
    TH1D* hRecoPrimaryCtr;
    TH1D* hRecoPrimaryPID;
  };
}
#endif

/** @} */ // end of doxygen group 
