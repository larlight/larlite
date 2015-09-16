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
#include "ERTool/Base/Particle.h"
#include "GeoAlgo/GeoAlgo.h"
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

   void ResetEventTree() ;
   void ResetPartTree() ;
   void ResetIntTree() ;

    //void CalculateAngleYZ( const Particle & p, double & angle ) ;
    void CalculateAngleYZ( const geoalgo::Point_t & p, double & angle ) ;

  private:

    unsigned int _run, _subrun, _event;

    TTree* _event_tree;
    unsigned int _ctr_cosmic;
    unsigned int _ctr_cosmic_w_secondaries;
    unsigned int _ctr_non_cosmic_w_sec;
    unsigned int _ctr_non_cosmic;
    unsigned int _ctr_interaction;
    unsigned int _ctr_orphan;
    
    TTree* _int_tree;
    double _int_x, _int_y, _int_z;
    int _primary_pdg;
    unsigned int _ctr_child;
    unsigned int _ctr_level;
    double _angle; 
    double _length ;

    TTree* _part_tree;
    int _pdg;
    bool _primary;
    double _start_x, _start_y, _start_z;
    double _end_x, _end_y, _end_z;
    double _px, _py, _pz;

    ::geoalgo::AABox fTPC ;
    ::geoalgo::GeoAlgo _geoAlgo;

    double _distToTopWall ;
    double _distBackAlongTraj ;
    double _distToWall ;
    
  };
}
#endif

/** @} */ // end of doxygen group 
