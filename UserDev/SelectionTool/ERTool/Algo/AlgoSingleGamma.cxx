/*
AlgoSingleGamma, July 2015

authors : Brooke Russell, brooke.russell@yale.edu
          Bobby Murrells, robertmurrells@gmail.com
	  Joseph Zennamo, jzennamo@uchicago.edu
*/

#ifndef ERTOOL_ALGOSINGLEGAMMA_CXX
#define ERTOOL_ALGOSINGLEGAMMA_CXX

#include "AlgoSingleGamma.h"

namespace ertool {

  size_t single_gam_NCcounter = 0;
  size_t mult_gam_NCcounter = 0;
  size_t single_gam_CCcounter = 0;

  AlgoSingleGamma::AlgoSingleGamma(const std::string& name) : AlgoBase(name)
  {
    _Ethreshold = 50; // set default energy threshold
    _verbose = false; // set verbosity to be off by default
    _useRadLength = false;
    _hassister = false;
    _vtx_min_radius = 20; // minimum bounding sphere radius in cm
  }

  void AlgoSingleGamma::Reset()
  {
    std::cout<<__FUNCTION__<<" found "<<single_gam_NCcounter<<
      " NC events with a single photon"<<std::endl;
    std::cout<<" and "<<mult_gam_NCcounter<<
      " NC events with multiple photons"<<std::endl;
    std::cout<<" and "<<single_gam_CCcounter<<
      " CC events with a single photon"<<std::endl;
  }

  void AlgoSingleGamma::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true); // true for gamma; false for e-

    if(!_e_ll_values)
      _e_ll_values = new TH1F("e_ll_values","e_ll_values",1000,-1,0);
    
    if(!_dedx_values)
      _dedx_values = new TH1F("dedx_values","dedx_values",1000,0,8);
    
    if (_alg_tree) { delete _alg_tree; }
    _alg_tree = new TTree("_alg_tree","Algo SingleGamma Tree");
    _alg_tree->Branch("_E",&_E,"_E/D");
    _alg_tree->Branch("_PDG",&_PDG,"PDG/I");
    _alg_tree->Branch("_VsTrack",&_VsTrack,"_VsTrack/I");
    _alg_tree->Branch("_thatE",&_thatE,"thatE/D");
    _alg_tree->Branch("_dEdx",&_dEdx,"_dEdx/D");
    _alg_tree->Branch("_IP",&_IP,"_IP/D");
    _alg_tree->Branch("_IPthisStart",&_IPthisStart,"_IPthisStart/D");
    _alg_tree->Branch("_IPthatStart",&_IPthatStart,"_IPthatStart/D");
    _alg_tree->Branch("_IPtrkBody",&_IPtrkBody,"_IPtrkBody/D");
    _alg_tree->Branch("_distBackAlongTraj",&_distBackAlongTraj,"_distBackAlongTraj/D");
    _alg_tree->Branch("_distToTopWall",&_distToTopWall,"_distToTopWall/D");
    _alg_tree->Branch("_Ngamma",&_Ngamma,"_Ngamma/I");
    _alg_tree->Branch("_Ntrks",&_Ntrks,"_Ntrks/I");
    _alg_tree->Branch("_Nmu",&_Nmu,"_Nmu/I");

    if(_empart_tree){delete _empart_tree;}
    _empart_tree = new TTree("_empart_tree","EMPart gamma/electron Tagging Tree");
    _empart_tree->Branch("_dedx",&_dedx,"dedx/D");
    _empart_tree->Branch("_radlen",&_radlen,"radlen/D");
    _empart_tree->Branch("_pdg",&_pdg,"pdg/I");
    
    return;
  }
  
  void AlgoSingleGamma::AcceptPSet(const ::fcllite::PSet& cfg) 
  {
    _alg_emp.AcceptPSet(cfg); // load EMPart parameters
    return;
  }

  bool AlgoSingleGamma::Reconstruct(const EventData &data, ParticleGraph& graph)
  { 
    auto datacpy = data; // copy EventData input

    if (_verbose)
      { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      }
    std::vector<int> showers_counted;
    std::vector<int> tracks_counted;

    /// ### STEP 1 ###
    /// loop thru particles associated with a shower object
    for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower))
      {
      /// get the shower object
      auto const& thisShower = datacpy.Shower(graph.GetParticle(s).RecoID());

      /// assign a vtx to this shower
      geoalgo::Point_t vtx_s(3);
	
	  /// ### STEP 2 ###
	  /// check if shower is above energy threshold
	  if(thisShower._energy < _Ethreshold)
	    {
	      continue;
	    }
	  /// ### STEP 3 ###
	  /// check to make sure the shower has not
	  /// already been added to a particle graph
	  if(std::find(std::begin(showers_counted),
		       std::end(showers_counted),s)
	     != std::end(showers_counted))
	    {
	      continue;
	    }

	  /// vector of showers vertices associated with this event
	  std::vector<int> shower_vtx;
	  shower_vtx.push_back(s);

	  /// ### STEP 4 ###
	  /// loop thru particles associated with a track object
	  for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack))
	    {
	      /// get the track object
	      auto const& thisTrack = datacpy.Track(graph.GetParticle(t).RecoID());

	      /// check to make sure the track has not already
	      /// been added to a particle graph
	      if(std::find(std::begin(tracks_counted),
			   std::end(tracks_counted),t)
		 != std::end(tracks_counted))
		{
		  continue;
		}
	      /// disregard tracks that are two short to be resolved
	      if(thisTrack.size() < 2)
		{
		  continue;
		}

	      /// assign a vtx to this track
	      geoalgo::Point_t vtx_t(3);
	      
	      /// ### STEP 5 ###
	      /// determine the impact parameter
	      /// between the shower and each track
	      double IPi = _findRel.FindClosestApproach(thisShower,
							thisTrack,
							vtx_s);

	      /// calculate distance between shower start point
	      /// and shower impact parameter mid point
	      auto const& thisShwrStart = thisShower.Start();
	      double disti = thisShwrStart.Dist(IPi);
	      
	      /// ### STEP 6 ###
	      /// using AlgoEMPart, proceed only if the
	      /// shower corresponds to a photon
	      if (_alg_emp.LL(false,thisShower._dedx,disti) >
		  (_alg_emp.LL(true,thisShower._dedx,disti)))
		{
		  continue;
		}

	      /// vector of vertices of tracks from a common origin area
	      std::vector<int> track_com_vtx;
	      track_com_vtx.push_back(t);
	      
	      /// ### STEP 7 ###
	      /// loop thru particles associated with a track but not "this" track
	      for (auto const& tr : graph.GetParticleNodes(RecoType_t::kTrack))
		{
		  /// get the track object
		  auto const& otherTrack =
		    datacpy.Track(graph.GetParticle(tr).RecoID());


		  /// check to see if track has already
		  ///been added to a particle graph
		  if(std::find(std::begin(tracks_counted),
			       std::end(tracks_counted),tr)
		     != std::end(tracks_counted))
		    {
		      continue;
		    }

		  /// disregard tracks that are too short to be resolved
		  if(otherTrack.size() < 2)
		    {
		      continue;
		    }

		  /// assign a vertex to this track
		  geoalgo::Point_t vtx_tr(3);

		  /// group vertices of tracks from a common origin area
		  track_com_vtx.push_back(tr);

		  /// ### STEP 8 ###
		  /// construct a sphere with minimum radius
		  /// bounded by the common track vertices
		  ::geoalgo::Sphere s(vtx_t,vtx_tr);

		  /// if radius of containment is smaller than minimum
		  /// value set in config file then proceed
		  if(s.Radius() < _vtx_min_radius)
		    {
		      _hRadius->Fill(s.Radius());
		      continue;
		    }

		  /// ### STEP 9 ###
		  /// define the center of the sphere as the "master" track vertex
		  /// sphCtr = s.Center();

		  /// ### STEP 10 ###
		  /// loop the particles associated with a shower object
		  for (auto const& sn : graph.GetParticleNodes(RecoType_t::kShower))
		    {
		      /// get the shower object
		      auto const& thatShower = datacpy.Shower(graph.GetParticle(sn).RecoID());
		      /// check if shower is above energy threshold
		      if (thatShower._energy < _Ethreshold)
			{
			  continue;
			}

		      /// check to make sure that shower has not
		      /// already been added to a particle graph
		      if(std::find(std::begin(showers_counted),
				   std::end(showers_counted),sn)
			 != std::end(showers_counted))
			{
			  continue;
			}

		      /// assign a vtx to this track
		      geoalgo::Point_t vtx_sn(3);

		      /// ### STEP 11 ###
		      /// determine the impact parameter between
		      /// this shower and that shower
		      double IPsn = _findRel.FindClosestApproach(thisShower,
								thatShower,
								vtx_sn);

		      /// calculate the distance between that shower start
		      /// point and the impact parameter mid point
		      auto const& thatShwrStart = thatShower.Start();
		      double dist_sn = thatShwrStart.Dist(IPsn);

		      /// using AlgoEMPart, proceed only if the
		      /// shower corresponds to a photon
		      if (_alg_emp.LL(false,thatShower._dedx,dist_sn) >
			  (_alg_emp.LL(true,thatShower._dedx,dist_sn)))
			{
			  continue;
			}
			  
		      /// vector of vertices of showers associated
		      /// with this shower/track event
		      shower_vtx.push_back(sn);
		    }
		}
	    }
      }
  }
  void AlgoSingleGamma::ProcessEnd(TFile* fout)
  {
    return;
  }


}
#endif
