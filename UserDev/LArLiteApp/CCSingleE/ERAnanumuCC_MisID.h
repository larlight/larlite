/**
 * \file ERAnanumuCC_MisID.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnanumuCC_MisID
 *
 * @author jzennamo
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANANUMUCC_MISID_H
#define ERTOOL_ERANANUMUCC_MISID_H

#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class ERAnanumuCC_MisID
     User custom Analysis class made by kazuhiro
   */
  class ERAnanumuCC_MisID : public AnaBase {
  
  public:

    /// Default constructor
    ERAnanumuCC_MisID();

    /// Default destructor
    virtual ~ERAnanumuCC_MisID(){}

    /// Reset function
    virtual void Reset();

    /// Called @ before processing the first event sample
    virtual void ProcessBegin(){}

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout);

  private: 
    //    TTree* _ana_tree;
    int _numEvents, _numMisID;
    int _MCelec, _RECOelec;
    int _MCphoton, _RECOphoton;
    int _MCmuon, _RECOmuon;
    int _MCpion, _RECOpion;
    int _RECOshowers, _RECOtracks, _RECOunknown;
    

  };
}
#endif

/** @} */ // end of doxygen group 
