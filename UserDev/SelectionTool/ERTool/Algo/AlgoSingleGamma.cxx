/*
AlgoSingleGamma, July 2015

authors : Brooke Russell, brooke.russell@yale.edu
          Bobby Murrells, robertmurrells@gmail.com
	  Joseph Zennamo, jzennamo@uchicago.edu
*/

/* ITEMS TO BE IMPLEMENTED (BR):
(1) iteratively loop thru tracks to form the most tightly bound --
based on impact parameter between each track
(2) check that all track endpoints are inside this sphere
(3) loop for showers which meet "halfline impact parameter" 
requirements with the center of this sphere
(4) also loop for showers which have a reasonable impact parameter
with initial shower
(5) develop chi^2 handle to determine which spheres are most highly
associated with vertex of event and which can be discarded
(6) fill histos & include more branches of interest
*/






#ifndef ERTOOL_ALGOSINGLEGAMMA_CXX
#define ERTOOL_ALGOSINGLEGAMMA_CXX

#include "AlgoSingleGamma.h"

namespace ertool {

  size_t single_gam_NCcounter = 0;
  size_t mult_gam_NCcounter = 0;
  size_t single_gam_CCcounter = 0;

  AlgoSingleGamma::AlgoSingleGamma(const std::string& name)
    : AlgoBase(name)
    , _e_ll_values(nullptr)
    , _dedx_values(nullptr)
    , _empart_tree(nullptr)
    , _alg_tree(nullptr)
    , _hRadius(nullptr)
    , _IPi(nullptr)
    , _IPj(nullptr)
    , _IPsn(nullptr)
  {
    _Ethreshold = 50; // set default energy threshold
    _verbose = false; // set verbosity to be off by default
    _useRadLength = false;
    _hassister = false;
    _vtx_min_radius = 20; // minimum bounding sphere radius in cm
    _maxIP = 20; // maximum impact parameter
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
    _alg_tree->Branch("_thatE",&_thatE,"_thatE/D");
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

    //    if(_hRadius)
    //_hRadius = new TH1D("_hRadius","_hRadius",1000,0,100);

    //if(_IPi)
    //_IPi = new TH1D("_IPi","_IPi",200,0,20);

    //if(_IPj)
    //_IPj = new TH1D("_IPj","_IPj",200,0,20);

    //if(_IPsn)
    //_IPsn = new TH1D("_IPsn","_IPsn",200,0,20);
    
    return;
  }
  
  void AlgoSingleGamma::AcceptPSet(const ::fcllite::PSet& cfg) 
  {
    _alg_emp.AcceptPSet(cfg); // load EMPart parameters
    return;
  }

  bool AlgoSingleGamma::Reconstruct(const EventData &data, ParticleGraph& graph)
  { 
    auto datacpy = data; 

    if (_verbose)
      { 
      std::cout << "***********BEGIN RECONSTRUCTION************" << std::endl;
      }
    
    std::vector<int> showers_counted;
    std::vector<int> tracks_counted;

    /// loop thru particles associated with a shower object
    for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower))
      {
	auto const& thisShower = datacpy.Shower(graph.GetParticle(s).RecoID());

	/// check to make sure the shower has not
	/// already been added to a particle graph
	if(std::find(std::begin(showers_counted),
		       std::end(showers_counted),s)
	   != std::end(showers_counted))
	  {
	    continue;
	  }

	/// mark this shower as counted
	showers_counted.push_back(s);
	
	if(thisShower._energy < _Ethreshold)
	  {
	    continue;
	  }
	
	geoalgo::Point_t vtx_s(3);

      	/// loop thru particles associated with a track object
	for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack))
	  {
	    auto const& thisTrack = datacpy.Track(graph.GetParticle(t).RecoID());

	    if(std::find(std::begin(tracks_counted),
			 std::end(tracks_counted),t)
	       != std::end(tracks_counted))
	      {
		continue;
	      }

	    tracks_counted.push_back(t);
	      
	    /// disregard tracks that are two short to be resolved
	    if(thisTrack.size() < 2)
	      {
		continue;
	      }
	      	     
	    geoalgo::Point_t vtx_t(3);

	    /// define a vector of vertices
	    std::cout << "1" << std::endl;
	    std::vector<::geoalgo::Point_t> pts = {vtx_t};
	    pts.clear();
	    pts.push_back(vtx_t);
	    std::cout << "1end" << std::endl;
	      
	    /// determine the impact parameter
	    /// between the shower and this track
	    double IPi = _findRel.FindClosestApproach(thisShower,
							thisTrack,
							vtx_s);

	    std::vector<double> impactPst;
	    impactPst.push_back(IPi);
	    
	    if (IPi < _maxIP)
	      {
		//_IPi->Fill(IPi);
		continue;
	      }
	  
	    /// calculate distance between shower start point
	    /// and impact parameter mid point
	    auto const& thisShwrStart = thisShower.Start();
	    double disti = thisShwrStart.Dist(vtx_s);

	    std::vector<int > otherTracksCounted;
	    
	    /// loop thru particles associated with a track but not "this" track
	    for (auto const& tr : graph.GetParticleNodes(RecoType_t::kTrack))
	      {
		auto const& otherTrack =
		  datacpy.Track(graph.GetParticle(tr).RecoID());

		if(std::find(std::begin(tracks_counted),
			     std::end(tracks_counted),tr)
		   != std::end(tracks_counted))
		  {
		    continue;
		  }

		otherTracksCounted.push_back(tr);
		
		if(otherTrack.size() < 2)
		  {
		    continue;
		  }

		geoalgo::Point_t vtx_tr(3);
		
	        pts.push_back(vtx_tr);

		double IPj = _findRel.FindClosestApproach(thisTrack,
							  otherTrack,
							  vtx_tr
							 );
		if (IPj < _maxIP)
		  {
		    //_IPj->Fill(IPj);
		    continue;
		  }
		
		std::vector<double> impactPtt;
		impactPtt.push_back(IPj);

		if (impactPtt.size() < 4)
		  {
		    continue;
		  }
		  
		/// construct a sphere with minimum radius
		/// bounded by the common track vertices
		std::cout << "calling sphere" << std::endl;
		::geoalgo::Sphere s(pts);
		std::cout << "called sphere" << std::endl;

	        if(s.Radius() < _maxIP)
		  {
		    _hRadius->Fill(s.Radius());
		    continue;
		  }

		double disto = thisShwrStart.Dist(s.Center());
				
		if (_alg_emp.LL(false,thisShower._dedx,disto) >
		    (_alg_emp.LL(true,thisShower._dedx,disto)))
	          {
		    continue;
		  }
		std::cout << "fff" << std::endl;
		/// loop the particles associated with a shower object
		for (auto const& sn : graph.GetParticleNodes(RecoType_t::kShower))
		  {
		    auto const& thatShower =
		      datacpy.Shower(graph.GetParticle(sn).RecoID());

		    if (thatShower._energy < _Ethreshold)
			{
			  continue;
			}
		    std::cout << "eee" << std::endl;
		    if(std::find(std::begin(showers_counted),
				 std::end(showers_counted),sn)
		       != std::end(showers_counted))
		       {
			 continue;
		       }
		    std::cout << "ddd" << std::endl;
 		    geoalgo::Point_t vtx_sn(3);
		    
		    double IPsn = _findRel.FindClosestApproach(thisShower,
								thatShower,
								vtx_sn);
		    if(IPsn < _maxIP)
		      {
			//_IPsn->Fill(IPsn);
			continue;
		      }

		      /// calculate the distance between that shower start
		      /// point and the impact parameter mid point
		      auto const& thatShwrStart = thatShower.Start();
		      double dist_sn = thatShwrStart.Dist(vtx_sn);
		    
		      if (_alg_emp.LL(false,thatShower._dedx,dist_sn) >
			  (_alg_emp.LL(true,thatShower._dedx,dist_sn)))
			{
			  continue;
			}
		      std::cout << "ccc" << std::endl;
		  }
	      }
	    std::cout << "bbb" << std::endl;
	  }
	std::cout << "aaa" << std::endl;
      }
    std::cout << "return.." << std::endl;
    return true;
  }
  void AlgoSingleGamma::ProcessEnd(TFile* fout)
  {
    return;
  }


}
#endif
