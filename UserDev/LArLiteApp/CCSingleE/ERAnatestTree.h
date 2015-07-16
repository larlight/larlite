/**
 * \file ERAnatestTree.h
 *
 * \ingroup CCSingleE
 * 
 * \brief Class def header for a class ERAnatestTree
 *
 * @author npereira
 */

/** \addtogroup CCSingleE

    @{*/

#ifndef ERTOOL_ERANATESTTREE_H
#define ERTOOL_ERANATESTTREE_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"
#include "ERTool/Base/ParticleGraph.h"

namespace ertool {

  /**
     \class ERAnatestTree
     User custom Analysis class made by kazuhiro
   */
  class ERAnatestTree : public AnaBase {
  
  public:

    /// Default constructor
    ERAnatestTree(const std::string& name="ERAnatestTree");

    /// Default destructor
    virtual ~ERAnatestTree(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
     void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    void PrepareTTree() ;

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  private: 
    
    /// TTree
    TTree* _tree;
    double _xMC;
    double _yMC;
    double _zMC;
    double _eMC;
    double _keMC;
    double _mMC;
    double _xReco;
    double _yReco;
    double _zReco;
    double _eReco;
    double _keReco;
    double _mReco;
    int counter;
    int event_counter;
  };

}
#endif

/** @} */ // end of doxygen group 
