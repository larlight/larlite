/*
AlgoSingleGamma, July 2015

authors : Brooke Russell, brooke.russell@yale.edu
          Bobby Murrells, robertmurrells@gmail.com
	  Joseph Zennamo, jzennamo@uchicago.edu
*/

/*
Ongoing implementation:
--- directionality of cones associated with showers 
and corresponding cut on cones which do not subtend
event vertex
--- chi^2 to choose from vertex candididates for 
event vertex AND chi^2 metric for shower assocition 
with event vertex
--- particle graph
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
    , _IPj(nullptr)
    , _IPsn(nullptr)
  {
    _Ethreshold = 50; // set default energy threshold
    _verbose = false; // set verbosity to be off by default
    _useRadLength = false;
    _hassister = false;
    _vtx_min_radius = 20; // minimum bounding sphere radius in cm
    _maxIP = 20; // maximum impact parameter

    track_gamma = 0;
    track_elec = 0;
    track_aelec = 0;
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
    /* ------------ */
    _alg_tree->Branch("_ThsShwrThsTrk",&_ThsShwrThsTrk,"_ThsShwrThsTrk/D");
    _alg_tree->Branch("_ThsShwrE",&_ThsShwrE,"_ThsShwrE/D");
    _alg_tree->Branch("_dedxThsShwr",&_dedxThsShwr,"_dedxThsShwr/D");
    _alg_tree->Branch("_ThsTrkE",&_ThsTrkE,"_ThsTrkE/D");
    _alg_tree->Branch("_IPst",&_IPst,"_IPst/D");
    _alg_tree->Branch("_IPstThsShwrSt",&_IPstThsShwrSt,"_IPstThsShwrSt/D");
    _alg_tree->Branch("_IPstThsTrkSt", &_IPstThsTrkSt,"_IPstThsTrkSt/D");
    _alg_tree->Branch("_IPstThsTrkBody", &_IPstThsTrkBody,"_IPstThsTrkBody/D");
    /* ------------ */
    _alg_tree->Branch("_ThsTrkOthTrk",&_ThsTrkOthTrk,"_ThsTrkOthTrk/D");
    _alg_tree->Branch("_OthTrkE",&_OthTrkE,"_OthTrkE/D");
    _alg_tree->Branch("_IPtt",&_IPtt,"IPtt/D");
    _alg_tree->Branch("_IPttOthTrkSt",&_IPttOthTrkSt,"_IPttOthTrkSt/D");
    _alg_tree->Branch("_IPttOthTrkBody",&_IPttOthTrkBody,"_IPttOthTrkBody/D");
    /* ------------ */
    _alg_tree->Branch("_ThsShwrOthShwr",&_ThsShwrOthShwr,"_ThsShwrOthShwr/D");
    _alg_tree->Branch("_OthShwrE",&_OthShwrE,"_OthShwrE/D");
    _alg_tree->Branch("_dedxOthShwr",&_dedxOthShwr,"_dedxOthShwr/D");
    _alg_tree->Branch("_IPss",&_IPss,"_IPss/D");
    _alg_tree->Branch("_IPssOthShwrSt",&_IPssOthShwrSt,"_IPssOthShwrSt/D");
    /* ------------ */
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

    //if(_IPst)
    //_IPst = new TH1D("_IPi","_IPi",200,0,20);

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
    std::vector<int> other_tracks_counted;
    std::vector<int> other_showers_counted;
    std::vector<int> pi0s_counted;

    for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower))
      {
	auto const& thisID = graph.GetParticle(s).RecoID();
	auto const& thisShower = datacpy.Shower(thisID);
	auto const& thisParent = graph.GetParticle(s).Parent();

	/*only consider showers that have either 
	  not been assessed or are Pi0 children*/
	if(graph.GetParticle(s).RelationAssessed())
	  {
	    if(graph.GetParticle(thisParent).PdgCode() != 111)
	      { continue; }
	  }
	
	/// disregard showers which are below energy threshold
	if(thisShower._energy < _Ethreshold)
	  { continue; }
		
	NodeID_t parent_track = -1;
	int parent_track_counter = 0;
	
	for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack))
	  {
	    auto const& thisTrack = datacpy.Track(graph.GetParticle(t).RecoID());
	    if(graph.GetParticle(t).RelationAssessed())
	      { continue; }

	    /// disregard tracks that are too short to be resolved
	    if(thisTrack.size() < 2)
	      { continue; }

	    std::vector<::geoalgo::Vector> vrtThsShwrThsTrk;
	    std::vector<::geoalgo::Vector> vrtThsTrkOthTrk;
	    geoalgo::Vector vtx_st(0,0,0);
	    geoalgo::Vector vtx_tt(0,0,0);
	    
	    /// determine the impact parameter
	    /// between this shower and this track
	    double IPst = _findRel.FindClosestApproach(thisShower,
							thisTrack,
							vtx_st);

	    if (IPst > _maxIP)
	      {	continue; }

	    vrtThsShwrThsTrk.push_back(vtx_st);
	    vrtThsTrkOthTrk.push_back(vtx_st);
	    
	    _ThsShwrThsTrk = 1;
	    _ThsShwrE = thisShower._energy;
	    _dedxThsShwr = thisShower._dedx;
	    _ThsTrkE = thisTrack._energy;
	    _IPst = IPst;
	    _IPstThsShwrSt = vtx_st.Dist(thisShower.Start()); // rad. length
	    _IPstThsTrkSt = vtx_st.Dist(thisTrack.front());
	    _IPstThsTrkBody = sqrt(_geoAlgo.SqDist(vtx_st,thisTrack));
	    _alg_tree->Fill();
	    
	    /// calculate distance between shower start point
	    /// and impact parameter mid point
	    auto const& thisShwrStart = thisShower.Start();
	    double distst = thisShwrStart.Dist(vtx_st);

	    if(parent_track_counter < 1)
	      {
		if(vtx_st.Dist(thisTrack.front()) > _vtxToTrkStartDist &&
		   IPst / 2 < _vtxToTrkDist &&
		   distst < _vtxToShrStartDist)
		  {
		    parent_track_counter++;
		    if( _alg_emp.LL(false,thisShower._dedx,distst) >
			_alg_emp.LL(true,thisShower._dedx,distst))
		      { parent_track = t; }
		  }
	      }

	    for (auto const& tr : graph.GetParticleNodes(RecoType_t::kTrack))
	      {
		auto const& otherTrack =
		  datacpy.Track(graph.GetParticle(tr).RecoID());
		if(graph.GetParticle(tr).RelationAssessed())
		  { continue; }
		if(otherTrack.size() < 2)
		  { continue; }
		
		double IPtt = _findRel.FindClosestApproach(thisTrack,
							  otherTrack,
							  vtx_tt);
		if (IPtt > _maxIP)
		  { continue; }

		if(parent_track_counter < 1)
		  {
		    geoalgo::Point_t vtx_so(3);
		    double IPso = _findRel.FindClosestApproach(thisShower,
							       otherTrack,
							       vtx_so);
		    double distso = thisShower.Start().Dist(vtx_so);
		    if(vtx_so.Dist(thisTrack.front()) > _vtxToTrkStartDist &&
				   IPso / 2 < _vtxToTrkDist &&
				   distso < _vtxToShrStartDist)
		      { parent_track_counter++; }
		    if(_alg_emp.LL(false,thisShower._dedx,distso) >
		       _alg_emp.LL(true,thisShower._dedx,distso))
		      { parent_track = t; }
		  }
	
		vtx_tt = otherTrack.front();
		vrtThsTrkOthTrk.push_back(vtx_tt);

		// TEMPORARY placement
		if(vrtThsTrkOthTrk.size()>4)
		  {continue;}
		
		/// construct a sphere with minimum radius
		/// bounded by the common track vertices
		::geoalgo::Sphere s(vrtThsTrkOthTrk);
		if(s.Radius() > _maxIP)
		  {
		    vrtThsTrkOthTrk.pop_back();
		    continue;
		  }

		_ThsTrkOthTrk = 1;
		_OthTrkE = otherTrack._energy;
		_IPtt = IPtt;
		_IPttOthTrkSt = vtx_tt.Dist(otherTrack.front());
		_IPttOthTrkBody = sqrt(_geoAlgo.SqDist(vtx_tt,otherTrack));
		_alg_tree->Fill();

		///COMPUTE CANDIDATE VERTEX CHI^2 HERE!!!
		
		/// radiation length: distance between the center
		/// of the sphere and the shower start point
		double disto = thisShwrStart.Dist(s.Center());

		if (_alg_emp.LL(true,thisShower._dedx,disto) >
		    (_alg_emp.LL(false,thisShower._dedx,disto)))
	          { continue; }
		std::cout << "fff" << std::endl;

		for (auto const& sn : graph.GetParticleNodes(RecoType_t::kShower))
		{
		  auto const& otherShower =
		    datacpy.Shower(graph.GetParticle(sn).RecoID());
		  if (otherShower._energy < _Ethreshold)
			{ continue; }
		  std::cout << "eee" << std::endl;
		  if (graph.GetParticle(sn).RelationAssessed())
		    {
		      if (graph.GetParticle(thisParent).PdgCode() != 111)
			  { continue; }
		    } 
		  std::cout << "ddd" << std::endl;

		  std::vector<::geoalgo::Vector> vrtThsShwrOthShwr;
		  //geoalgo::Vector vtx_ss(0,0,0);
		  geoalgo::Point_t vtx_ss(3);
		  
		  double IPss = _findRel.FindClosestApproach(thisShower,
							     otherShower,
							     vtx_ss);
		    if(IPss > _maxIP)
		      { continue; }

		    //geoalgo::Vector dir = otherShower.Dir(vtx_ss);
		    auto dir = otherShower.Dir();
		    auto ln = otherShower.Length();
		    auto radius = otherShower.Radius();
		    geoalgo::Cone c(vtx_ss,dir,ln,radius);

		    vrtThsShwrOthShwr.push_back(vtx_ss);
		    
		      /// calculate the distance between that shower start
		      /// point and the impact parameter mid point
		      auto const& otherShwrStart = otherShower.Start();
		      double dist_ss = otherShwrStart.Dist(vtx_ss);

		      if (_alg_emp.LL(true,otherShower._dedx,dist_ss) >
			  (_alg_emp.LL(false,otherShower._dedx,dist_ss)))
			{ continue; }
		      std::cout << "ccc" << std::endl;

		      _ThsShwrOthShwr = 1;
		      _OthShwrE = otherShower._energy;
		      _dedxOthShwr = otherShower._dedx;
		      _IPss = IPss;
		      _IPssOthShwrSt = vtx_ss.Dist(otherShower.Start());
		      _alg_tree->Fill();
		} // <-- end for loop sn
	      } // <-- end for loop tr
	    std::cout << "bbb" << std::endl;
	  } // <-- end for loop t
	if(parent_track != -1 && parent_track_counter == 1)
	  {
	    graph.SetParentage(parent_track, s);
	    switch(graph.GetParticle(s).PdgCode())
	      {
	      case 22:
		track_gamma++;
		break;
	      case 11:
		track_elec++;
		break;
	      case -11:
	        track_aelec++;
		break;
	      default:
		std::cout << "Warning, shower pdg: "
			  << graph.GetParticle(s).PdgCode() << "\n";
	      } 
	  } 
	std::cout << "aaa" << std::endl;
      } // <-- end for loop s
    std::cout << graph.Diagram();
    std::cout << "return.." << std::endl;
    return true;
  } // <-- end Reconstruct
  void AlgoSingleGamma::ProcessEnd(TFile* fout)
  {
    std::cout << "Track children identified as gammas:\n\tgamma: "
	      << track_gamma << " "
	      << " elec: " << track_elec
	      << " aelec: " << track_aelec << "\n";
    
    return;
  }

}
#endif
