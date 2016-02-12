/**
 * \file ERAlgoVertexBuilder.h
 *
 * \ingroup main
 * 
 * \brief Class def header for a class ERAlgoVertexBuilder
 *
 * @author rmurrell
 */

/** \addtogroup main

    @{*/

#ifndef ERTOOL_ERALGOVERTEXBUILDER_H
#define ERTOOL_ERALGOVERTEXBUILDER_H

#include "ERTool/Base/AlgoBase.h"

#include <list>

#include "GeoAlgo/GeoAABox.h"
#include "GeoAlgo/GeoAlgo.h"

#include "TTree.h"

namespace ertool {

  /**
     \class ERAlgoVertexBuilder
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoVertexBuilder : public AlgoBase {

    Double_t const tstart_prox;
    Double_t const tmax_rad;
    Double_t const tlone_track_length;
    Bool_t const twithTrackDir;
    Bool_t const tverbose;

    TTree * tree;

    Int_t event_id;
    Int_t association_number;
    Int_t found_vertices;
    Int_t loop_counter;
    Int_t lone_track_counter;
    Int_t vertices_lonetracks;
    Double_t radius;

    geoalgo::AABox volume;
    geoalgo::GeoAlgo const algo;

  public:

    /// Default constructor
    ERAlgoVertexBuilder(Double_t const start_prox,
			Double_t const max_rad,
			Double_t const lone_track_length,
			Bool_t const withTrackDir = true,
			const std::string& name="ERAlgoVertexBuilder");

    /// Default destructor
    virtual ~ERAlgoVertexBuilder(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    class ParticleAssociation;
    class ParticleAssociations;
    class ParticleGraphSetter;

    friend ParticleAssociation;
    friend ParticleAssociations;
    friend ParticleGraphSetter;

    void EndReconstruct(ParticleGraph const & graph);

    geoalgo::Point_t const * GetUpstreamPrimary
      (ParticleAssociations const & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index);

    geoalgo::Point_t const * GetMostChildrenPrimary
      (ParticleAssociations const & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index);

    geoalgo::Point_t const * GetSmallestSpherePrimary
      (ParticleAssociations const & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index);

    geoalgo::Point_t const * GetMostEnergyPrimary
      (EventData const & data,
       ParticleGraph const & graph,
       ParticleAssociations const & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index);
    
    void AddAllLoneTracks
      (const EventData &data,
       ParticleGraph & graph,
       ParticleAssociations const & pas);

    void AddUpstreamLoneTrack
      (const EventData &data,
       ParticleGraph & graph,
       ParticleAssociations const & pas); 
    
    void EndReconstructPa(const EventData &data,
			  ParticleGraph & graph,
			  ParticleAssociations const & pa);

    void WithTrackDir(const EventData &data, ParticleGraph& graph);

    void Erase(EventData const & data,
	       ParticleGraph const & graph,
	       std::multimap<NodeID_t, const geoalgo::Point_t *> & pn,
	       std::multimap<NodeID_t, const geoalgo::Point_t *>::iterator best_it,
	       geoalgo::Point_t const & sv);

    void WithoutTrackDir(const EventData &data, ParticleGraph& graph);

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
