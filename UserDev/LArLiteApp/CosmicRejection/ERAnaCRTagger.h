/**
 * \file ERAnaCRTagger.h
 *
 * \ingroup CosmicRejection
 * 
 * \brief Class def header for a class ERAnaCRTagger
 *
 * @author kazuhiro
 */

/** \addtogroup CosmicRejection

    @{*/

#ifndef ERTOOL_ERANACRTAGGER_H
#define ERTOOL_ERANACRTAGGER_H

#include "ERTool/Base/AnaBase.h"
#include <TTree.h>
namespace ertool {

  /**
     \class ERAnaCRTagger
     User custom Analysis class made by kazuhiro
   */
  class ERAnaCRTagger : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaCRTagger(const std::string& name="ERAnaCRTagger");

    /// Default destructor
    virtual ~ERAnaCRTagger(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  private:

    unsigned int _run, _subrun, _event;

    TTree* _event_tree;
    unsigned int _ctr_cosmic;
    unsigned int _ctr_non_cosmic;
    unsigned int _ctr_interaction;
    unsigned int _ctr_orphan;
    
    TTree* _int_tree;
    double _int_x, _int_y, _int_z;
    int _primary_pdg;
    unsigned int _ctr_child;
    unsigned int _ctr_level;

    TTree* _part_tree;
    int _pdg;
    double _x, _y, _z;
    double _ex, _ey, _ez;
    double _px, _py, _pz;
    
  };
}
#endif

/** @} */ // end of doxygen group 
