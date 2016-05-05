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

#include "ERTool/Algo/AlgoFindRelationship.h"

#include "TTree.h"

namespace ertool {

  class ERAlgoVertexBuilder : public AlgoBase {

    Double_t const tstart_prox;
    Double_t const tmax_rad;
    Bool_t const twithTrackDir;
    std::string const tprimary_vertex_selection;    
    Bool_t const tshowerproj;
    Bool_t const tshowerdir;
    Double_t const tshower_prox;
    Double_t const tcpoa_vert_prox;
    Double_t const tcpoa_trackend_prox;
    Bool_t tverbose;

    TTree * tree;

    Int_t event_id;
    Int_t association_number;
    Int_t found_vertices;
    Int_t loop_counter;
    Int_t lone_track_counter;
    Int_t vertices_lonetracks;
    Double_t radius;
    Int_t multi_track_end_vertex;

    geoalgo::AABox volume;
    geoalgo::GeoAlgo const algo;
   
    AlgoFindRelationship const findrel;

    std::string const mostupstream = "mostupstream";
    std::string const mostchildren = "mostchildren";
    std::string const mostenergy = "mostenergy";
    std::string const mostenergy_lone = "mostenergylone";
    std::string const smallestsphere = "smallestsphere";
    std::string const trackdirection = "trackdirection";

  public:

    /// Default constructor
    ERAlgoVertexBuilder
      (Double_t const start_prox,
       Double_t const max_rad,
       Bool_t const withTrackDir,
       std::string const primary_vertex_selection = "mostupstream",
       Bool_t const showerproj = false,
       Bool_t const showerdir = false,
       Double_t const shower_prox = 0,
       Double_t const cpoa_vert_prox = 0,
       Double_t const cpoa_trackend_prox = 0,
       const std::string& name = "ERAlgoVertexBuilder");
       
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

    geoalgo::Point_t const * SubGetUpstream
      (ParticleAssociations const & pas,
       std::vector<Int_t> const & indices,
       Int_t & index);

    geoalgo::Point_t const * SubSmallestSphere
      (ParticleAssociations const & pas,
       std::vector<Int_t> const & indices,
       Int_t & index);

    geoalgo::Point_t const * SubMostEnergy
      (EventData const & data,
       ParticleGraph const & graph,
       ParticleAssociations const & pas,
       std::vector<Int_t> const & indices,
       Int_t & index);

    geoalgo::Point_t const * GetMostChildrenPrimary
      (EventData const & data,
       ParticleGraph const & graph,
       ParticleAssociations const & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index);

    geoalgo::Point_t const * GetSmallestSpherePrimary
      (ParticleAssociations const & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index);

    geoalgo::Point_t const * GetMostEnergyPrimary
      (EventData const & data,
       ParticleGraph & graph,
       ParticleAssociations & pas, 
       std::vector<Int_t> const & skip,
       Int_t & index,
       Bool_t const lone = false,
       Bool_t * first = nullptr);

    void AddAllLoneTracks
      (const EventData &data,
       ParticleGraph & graph,
       NodeID_t const n = kINVALID_NODE_ID);

    void AddUpstreamLoneTrack
      (const EventData &data,
       ParticleGraph & graph);

    void AddAllLoneShowers
      (const EventData &data,
       ParticleGraph & graph,
       NodeID_t const n = kINVALID_NODE_ID);

    void AddTracksAndShowersUpstream
      (const EventData &data,
       ParticleGraph & graph);

    void AddTracksAndShowersEnergy
      (const EventData &data,
       ParticleGraph & graph);

    void ShowerProjection
      (const EventData &data,
       ParticleGraph & graph,
       ParticleAssociations & pas);

    void EndReconstructPa(const EventData &data,
			  ParticleGraph & graph,
			  ParticleAssociations & pa);

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
