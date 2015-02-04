/**
 * \file ERAnaSingleE.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnaSingleE
 *
 * @author david
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANASINGLEE_H
#define ERTOOL_ERANASINGLEE_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"

namespace ertool {

  /**
     \class ERAnaSingleE
     User custom Analysis class made by kazuhiro
   */
  class ERAnaSingleE : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaSingleE();

    /// Default destructor
    virtual ~ERAnaSingleE(){};

    /// Reset function
    virtual void Reset();

    /// Function to evaluate input showers and determine a score
    virtual bool Analyze(const EventData &data, const ParticleSet &ps);

    
    void ProcessEnd(TFile* fout);

  private:

    TTree* _ana_tree;
    int _singleRECO, _singleMC;
    int _RECOshowers, _RECOtracks;
    int _MCelectrons, _MCgammas;

    int _numEvts;
    int _singleE;

  };
}
#endif

/** @} */ // end of doxygen group 
