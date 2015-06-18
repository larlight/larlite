/**
 * \file ERAlgoPrimaryPi0.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoPrimaryPi0
 *
 * @author ariana Hackenburg
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOPRIMARYPI0_H
#define ERTOOL_ERALGOPRIMARYPI0_H

#include "ERTool/Base/AlgoBase.h"
#include "AlgoPrimaryFinder.h"

namespace ertool {

  /**
     \class ERAlgoPrimaryPi0
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoPrimaryPi0 : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoPrimaryPi0(const std::string& name="ERAlgoPrimaryPi0");

    /// Default destructor
    virtual ~ERAlgoPrimaryPi0(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void SetVerbose ( bool verbose ) { _verbose = verbose ; }
    
    void SetMinDistVtx   ( double minDistVtx ) { _minDistVtx = minDistVtx ; }
    void SetMinDistEnd   ( double minDistEnd ) { _minDistEnd = minDistEnd ; }

    /// Functions that returns if object given as 1st 
    ///argument comes from object given as second argument
//    bool From(const ::geoalgo::Trajectory& thisTrack,
  //            const ::geoalgo::Trajectory& thatTrack) const;

  //  bool From(const ::geoalgo::Trajectory& thisTrack,
    //          const ::geoalgo::Cone& thatShower) const;


  protected:

  AlgoPrimaryFinder _primary_alg ;
  
  bool 		_verbose    ;
  double 	_vtxDist    ;
  double 	_minDistVtx ;
  double 	_minDistEnd ;


  };
}
#endif

/** @} */ // end of doxygen group 
