#ifndef ERTOOL_ERALGONGAMMA_CXX
#define ERTOOL_ERALGONGAMMA_CXX

#include "ERAlgoNGamma.h"
#include <sstream>
namespace ertool {
  ERAlgoNGamma::ERAlgoNGamma(const std::string& name)
    : AlgoBase(name)
    , fTPC(-10.,-126.,-10.,292.,136.,1150.)
    , _empart_tree(nullptr)
    , _alg_tree(nullptr)
  {
    _gamma_mass = ParticleMass(22);
    _Ethreshold = 0;
  }

  void ERAlgoNGamma::Reset()
  {}

  void ERAlgoNGamma::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    /// load EMPart parameters
    _alg_emp.AcceptPSet(cfg);
    return;
  }

  void ERAlgoNGamma::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    _alg_emp.SetMode(true);

    if(_alg_tree) {delete _alg_tree;}
    _alg_tree = new TTree("_alg_tree","ER Algo N Gamma Tree");
    _alg_tree->Branch("_E",&_E,"_E/D");
    _alg_tree->Branch("_thatE",&_thatE,"_thatE/D");
    _alg_tree->Branch("_PDG",&_PDG,"_PDG/I");
    _alg_tree->Branch("_dEdx",&_dEdx,"_dEdx/D");
    _alg_tree->Branch("_VsTrack",&_VsTrack,"_VsTrack/I");
    _alg_tree->Branch("_IP",&_IP,"_IP/D");
    _alg_tree->Branch("_IPthisStart",&_IPthisStart,"_IPthisStart/D");
    _alg_tree->Branch("_IPthatStart",&_IPthatStart,"_IPthatStart/D");
    _alg_tree->Branch("_IPtrkBody",&_IPtrkBody,"_IPtrkBody/D");

    if(_empart_tree) {delete _empart_tree;}
    _empart_tree = new TTree("_empart_tree","EMPart gamma/electron Tagging Tree");
    _empart_tree->Branch("_dedx",&_dedx,"dedx/D");
    _empart_tree->Branch("_radlen",&_radlen,"radlen/D");
    _empart_tree->Branch("_pdg",&_pdg,"pdg/I");

    return;
  }

  bool ERAlgoNGamma::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    int ctr = 0;
    int gamma_evtctr = 0;
    int null_evtctr = 0;
    auto datacpy = data;
    if(Debug())
      {
	std::stringstream ss;
	ss<<"***** BEGIN N GAMMA RECONSTRUCTION *****"<<std::endl;
	ss<<"\tShowers in event : "<<data.Shower().size()<<std::endl;
	ss<<"\tTracks in event : "<<data.Track().size()<<std::endl;
	Debug(__FUNCTION__,ss.str());
      }
    std::vector<NodeID_t> sibShwrFin;
    std::vector<NodeID_t> siblingsS;
    std::vector<NodeID_t> siblingsT;
    RecoID_t refT,refS;
    Track referenceTrack;
    Shower referenceShower;
    double IPst=0;
    double radst=0;
    double IPsos=0;
    double radsos=0;
    geoalgo::Trajectory_t refTrk;
    geoalgo::Cone refShwr;
    // SECTION A ==============================================================
    for(auto const& s : graph.GetParticleNodes(RecoType_t::kShower))
      {
	auto const& thisID = graph.GetParticle(s).RecoID();
	auto const& thisShower = datacpy.Shower(thisID);
	auto const& thisParent = graph.GetParticle(s).Parent();
	/// only consider showers that have either not been
	/// assessed or are Pi0 children
	if(graph.GetParticle(s).RelationAssessed())
	  {
	    if(graph.GetParticle(thisParent).PdgCode() != 111) {continue;}
	  }
	/// disregard showers below energy threshold
	if(thisShower._energy < _Ethreshold) {continue;}
	::geoalgo::Vector cdir = thisShower.Dir();
	cdir = cdir*-1;
	/// construct a cone with same opening angle and axis projected
	/// backwards from this reference shower
	::geoalgo::Cone cref(thisShower.Start(),
			     cdir,
			     thisShower.Length(),
			     thisShower.Radius());
	/// opening angle of cone	
	double crefopang = atan(cref.Radius()/cref.Length());
	::geoalgo::HalfLine refhl(thisShower.Start(),
				  cdir);
	/// variables to be used in section E
	auto const& thisShwrStart = thisShower.Start();
	std::vector<geoalgo::Point_t> vtxT;
	std::vector<geoalgo::Point_t> vtxS;
	/// add this shower's NodeID_t to a shower sibling vector
	siblingsS.push_back(thisID);
	// SECTION B =========================================================
	/// find the track with the smallest impact parameter
	/// with respect to the reference shower
	double IPminA = 1036;
	::geoalgo::Point_t vtxMinA(3);
	for(auto const& t : graph.GetParticleNodes(RecoType_t::kTrack))
	  {
	    auto const& thisT = graph.GetParticle(t).RecoID();
	    //	    auto const& tT = graph.GetParticle(t).ID();
	    auto const& thisTrack = datacpy.Track(thisT);	    
	    /// only consider tracks that have not been assessed
	    if(graph.GetParticle(t).RelationAssessed()) {continue;}
	    /// only consider tracks that are long enough to be resolved
	    if(thisTrack.size() < 2) {continue;}
	    ::geoalgo::Point_t vtxst(3);
	    IPst = _findRel.FindClosestApproach(thisShower,
						thisTrack,
						vtxst);
	    if(IPst < IPminA) {IPminA = IPst; vtxMinA = vtxst;};
	    /// *** determine error on shwr-trk impact parameter ***
	    /// closest point between point and half line
	    ::geoalgo::Point_t refPtst = _geoAlgo.ClosestPt(vtxMinA,refhl);
	    auto const& hlStartst = refhl.Start();
	    /// determine the projected distance alog the axis at which
	    /// the closest point lies
	    double distst = hlStartst.Dist(refPtst);
	    /// determine the radius of the cone at the specified length
	    /// along the cone axis
	    radst = distst*(tan(crefopang)); // <-- shwr-trk error
	    /// add this track's NodeID_t to a track sibling vector
	    siblingsT.push_back(thisT);
	    refT = thisT;
	    referenceTrack = thisTrack;
	  } // <-- end t loop
	/// find the other shower with the smallest impact parameter
	/// with respect to the reference shower
	double IPminB = 1036;
	::geoalgo::Point_t vtxMinB(3);
	for(auto const& os : graph.GetParticleNodes(RecoType_t::kShower))
	  {
	    auto const& otherS = graph.GetParticle(os).RecoID();
	    //auto const& sS = graph.GetParticle(os).ID();
	    auto const& otherShower = datacpy.Shower(otherS);
	    /// check that the shower is not being compared against itself
	    if(os == s) {continue;}
	    auto const& othShwrParent = graph.GetParticle(os).Parent();
	    if(graph.GetParticle(os).RelationAssessed())
	      {if(graph.GetParticle(othShwrParent).PdgCode() != 111) {continue;}
	      }
	    ::geoalgo::Point_t vtxsos(3);
	    IPsos = _findRel.FindClosestApproach(thisShower,
						 otherShower,
						 vtxsos);
	    if(IPsos < IPminB) {IPminB = IPsos; vtxMinB = vtxsos;}
	    /// *** determine error on shwr-shwr impact parameter ***
	    geoalgo::Point_t refPtorigs = _geoAlgo.ClosestPt(vtxMinB,refhl);
	    auto const& hlStartorigs = refhl.Start();
	    double distorigs = hlStartorigs.Dist(refPtorigs);
	    double radorigs = distorigs*(tan(crefopang)); // <-- orig shwr error
	    ::geoalgo::Vector cosdir = otherShower.Dir();
	    cosdir = cosdir*-1;
	    /// construct a cone with same opening angle and axis
	    /// projected backwards from "other shower"
	    ::geoalgo::Cone coths(otherShower.Start(),
				  cosdir,
				  otherShower.Length(),
				  otherShower.Radius());
	    /// opening angle of other cone
	    double cothsopang = atan(coths.Radius()/coths.Length());
	    geoalgo::HalfLine othshl(otherShower.Start(),
				     cosdir);
	    ::geoalgo::Point_t refPtoths = _geoAlgo.ClosestPt(vtxMinB,othshl);
	    auto const& hlStartoths = othshl.Start();
	    double distoths = hlStartoths.Dist(refPtoths);
	    double radoths = distoths*(tan(cothsopang)); // <-- other shwr error
	    radsos = sqrt(radorigs*radorigs
			  +radoths*radoths); // <-- shwr-shwr error
	    /// variables to be used in section ___
	    /// add this shower's NodeID_t to a shower sibling vector
	    siblingsS.push_back(otherS);
	    geoalgo::Cone temp(otherShower.Start(),
			       otherShower.Dir(),
			       otherShower.Length(),
			       otherShower.Radius());
	    refShwr = temp;
	    refS = otherS;
	    referenceShower = otherShower;
	  } // <-- end os loop
	// SECTION C ==========================================================
	/// compare shwr-trk minimum IP with trk-trk minimum IP
	/// choose object with smaller IP to ossiciate with original shwr
	if(IPminA < IPminB && IPminA < _maxIP) // track preferred
	  {
	    siblingsS.pop_back();
	    for(auto const& ot : graph.GetParticleNodes(RecoType_t::kTrack))
	      {
		auto const& otherT = graph.GetParticle(ot).RecoID();
		auto const& otherTrack = datacpy.Track(otherT);
		//if (otherTrack == thisTrack) {continue;}
		if(graph.GetParticle(ot).RelationAssessed()) {continue;}
		if(otherTrack.size() < 2) {continue;}
		if(refT == otherT) {continue;}
		::geoalgo::Point_t vtxtot(3);
		double IPtot = _findRel.FindClosestApproach(referenceTrack,
							    otherTrack,
							    vtxtot);
		/// impose condition on impact parameter 
		if(IPtot > IPst+radst) {continue;}
		vtxT.push_back(vtxtot);
		//_E;
		_thatE = otherTrack._energy;
		//_PDG;
		//_dEdx;
		_VsTrack = 1;
		_IP = IPtot;
		_IPthisStart = vtxtot.Dist(thisShower.Start());
		_IPthatStart = vtxtot.Dist(otherTrack.front());
		//_IPtrkBody;
		_alg_tree->Fill();
		siblingsT.push_back(otherT);
	      } // <-- end ot loop
	    for(auto const& oos : graph.GetParticleNodes(RecoType_t::kShower))
	      {
		auto const& othothS = graph.GetParticle(oos).RecoID();
		auto const& othothShower = datacpy.Shower(othothS);
		auto const& othothShowerParent =
		  graph.GetParticle(oos).Parent();
		if(othothShower._energy<_Ethreshold) {continue;}
		if(graph.GetParticle(oos).RelationAssessed())
		  {
		    if(graph.GetParticle(othothShowerParent).PdgCode() != 111)
		      {continue;}
		  }
		if(thisID == othothS) {continue;}
		::geoalgo::Point_t vtxtoos(3);
		double IPtoos =
		  _findRel.FindClosestApproach(referenceTrack,
					       othothShower,
					       vtxtoos);
		///  impose condition on impact parameter 
		if(IPtoos > IPst+radst) {continue;}
		vtxT.push_back(vtxtoos);
		_VsTrack = 0;
		_thatE = othothShower._energy;
		_IP = IPtoos;
		_IPthisStart = vtxtoos.Dist(thisShower.Start());
		_IPthatStart = vtxtoos.Dist(othothShower.Start());
		_alg_tree->Fill();
		siblingsS.push_back(othothS);
	      } // <-- end oos loop
	  } // <-- end if(IPminA < IPminB)
	// SECTION D ===========================================================
	else // shower preferred
	  {
	    if(IPminB < _maxIP)
	      {
		siblingsT.pop_back();
		for(auto const& oot : graph.GetParticleNodes(RecoType_t::kTrack))
		  {
		    auto const& othothT = graph.GetParticle(oot).RecoID();
		    auto const& othothTrack = datacpy.Track(othothT);
		    if(graph.GetParticle(oot).RelationAssessed()) {continue;}
		    if(othothTrack.size() < 2) {continue;}
		    ::geoalgo::Point_t vtxsoot(3);
		    double IPsoot = _findRel.FindClosestApproach(referenceShower,
								 othothTrack,
								 vtxsoot);
		    /// *** impose condition on impact parameter ***
		    if(IPsoot > IPsos + radsos) {continue;}
		    vtxS.push_back(vtxsoot);
		    //_E;
		    _thatE = othothTrack._energy;
		    //_PDG;
		    //_dEdx;
		    _VsTrack = 1;
		    _IP = IPsoot;
		    _IPthisStart = vtxsoot.Dist(thisShower.Start());
		    _IPthatStart = vtxsoot.Dist(othothTrack.front());
		    //_IPtrkBody;
		    _alg_tree->Fill();
		    siblingsT.push_back(othothT);
		  } // <-- end oot loop
		for(auto const& ooos : graph.GetParticleNodes(RecoType_t::kShower))
		  {
		    auto const& othothothS = graph.GetParticle(ooos).RecoID();
		    auto const& othothothShower = datacpy.Shower(othothothS);
		    auto const& othothothShowerParent =
		      graph.GetParticle(ooos).Parent();
		    if(othothothShower._energy<_Ethreshold) {continue;}
		    if(graph.GetParticle(ooos).RelationAssessed())
		      {
			if(graph.GetParticle(othothothShowerParent).PdgCode()
			   != 111) {continue;}		     
		      }
		    if(refS == othothothS) {continue;}
		    if(thisID == othothothS) {continue;}
		    ::geoalgo::Point_t vtxsooos(3);
		    double IPsooos =
		      _findRel.FindClosestApproach(referenceShower,
						   othothothShower,
						   vtxsooos);
		    /// *** impose condition on impact parameter ***
		    if(IPsooos > IPsos + radsos) {continue;}
		    vtxS.push_back(vtxsooos);
		    _VsTrack = 0;
		    _thatE = othothothShower._energy;
		    _IP = IPsooos;
		    _IPthisStart = vtxsooos.Dist(thisShower.Start());
		    _IPthatStart = vtxsooos.Dist(othothothShower.Start());
		    _alg_tree->Fill();
		    siblingsS.push_back(othothothS);
		  } // <-- end ooos loop
	      } // <-- if (IPminB < _maxIP)
	  } // <-- end else
	// SECTION E ==========================================================
	///REMOVE IF STATEMENT ONCE "SPHERE" IS FIXED
	if(vtxT.size() < 4)
	  {
	    ::geoalgo::Sphere sT(vtxT);
	    const auto VTX = sT.Center();
	    /// define the radiation length of the original shower
	    // as the distance between the center of the sphere
	    ///and the shower start point
	    double distT = thisShwrStart.Dist(VTX);
	    if(_alg_emp.LL(true,thisShower._dedx,distT) >
	       (_alg_emp.LL(false,thisShower._dedx,distT))) {continue;}
	    _dedx = VTX.Dist(thisShower.Start());
	    _radlen = VTX.Dist(thisShower.Start());
	    _pdg = 22;
	    _empart_tree->Fill();
	    for(auto const& aa : siblingsS)
	      {
		/// disregard electrons in event
		auto const& bb = datacpy.Shower(graph.GetParticle(aa).RecoID());
		auto const& bbShwrStart = bb.Start();
		double distBB = bbShwrStart.Dist(VTX);
		if(_alg_emp.LL(true,bb._dedx,distBB) >
		   (_alg_emp.LL(false,bb._dedx,distBB))) {continue;}
		sibShwrFin.push_back(siblingsS[aa]);
		_dedx = bb._dedx;
		_radlen = VTX.Dist(bb.Start());
		_pdg = 22;
		_empart_tree->Fill();
	      } // <-- end aa loop
	  } // <-- end  if vtxT
	///REMOVE IF STATEMENT ONCE "SPHERE" IS FIXED
	if(vtxS.size() < 4)
	  {
	    ::geoalgo::Sphere sS(vtxS);
	    const auto VTX = sS.Center();
	    double distS = thisShwrStart.Dist(VTX);
	    if(_alg_emp.LL(true,thisShower._dedx,distS) >
	       (_alg_emp.LL(false,thisShower._dedx,distS))) {continue;}
	    _dedx = VTX.Dist(thisShower.Start());
	    _radlen = VTX.Dist(thisShower.Start());
	    _pdg = 22;
	    _empart_tree->Fill();
	    for(auto const& a : siblingsS)
	      {
		/// disregard electrons in event
		auto const& b =
		  datacpy.Shower(graph.GetParticle(a).RecoID());
		auto const& bShwrStart = b.Start();
		double distB = bShwrStart.Dist(VTX);
		if(_alg_emp.LL(true,b._dedx,distB) >
		   (_alg_emp.LL(false,b._dedx,distB))) {continue;}
		sibShwrFin.push_back(siblingsS[a]);
		_dedx = b._dedx;
		_radlen = VTX.Dist(b.Start());
		_pdg = 22;
		_empart_tree->Fill();
	      } // <-- end a loop
	  } // <-- end if vtxS
	// SECTION F ==========================================================
	/// fill the particle graph
	/// declare reference shower as a gamma
	auto& gamma = graph.GetParticle(s);
	double mom = thisShower._energy;
	gamma.SetParticleInfo(22,
			      _gamma_mass,
			      thisShower.Start(),
			      thisShower.Dir()*mom);
	for(auto const& c : sibShwrFin)
	  {
	    if (s == c) {continue;}
	    auto const& d = datacpy.Shower(graph.GetParticle(c).RecoID());
	    /// add this shower to ParticleTree
	    auto &ruggles = graph.GetParticle(c);
	    double shwrMom = d._energy;
	    ruggles.SetParticleInfo(22,
				    _gamma_mass,
				    d.Start(),
				    d.Dir()*shwrMom);
	    //graph.SetSiblings(s,c);
	  } // <-- end c loop 
	if(siblingsT.size()>0)
	  {
	    for(auto const& f : siblingsT)
	      {
		auto const& g = datacpy.Track(graph.GetParticle(f).RecoID());
		auto &charlesmgh = graph.GetParticle(f);
		// track deposited energy
		double EDep = g._energy;
		// track direction
		geoalgo::Vector_t Tdir = (g[1]-g[0]);
		Tdir.Normalize();
		double mass = _findRel.GetMass(g);
		geoalgo::Vector_t tMom = Tdir*(sqrt(EDep*(EDep+2*mass)));
		charlesmgh.SetParticleInfo(charlesmgh.PdgCode(),
					   mass,
					   g[0],
					   tMom);
		//graph.SetSiblings(s,f);
					   } // <-- end f loop
	  } //<-- end siblingsT.size()
	/// create a new particle for the neutrino
	/*if(_verbose)
	  {
	    std::cout<<"# of particles before: "<<graph.GetNumParticles()<<std::endl
		     <<"Making neutrino..."<<std::endl;
	    
	  }
	Particle& neutrino = graph.CreateParticle();
	if(_verbose)
	  {
	    std::cout<<"Made neutrino with ID: "<<neutrino.ID()<<" and PDG: "<<neutrino.PdgCode()<<std::endl
		     <<"# of particles after: "<<graph.GetNumParticles()<<std::endl;
	  } */
	/// set relationship
	//graph.SetParentage(neutrino.ID(),s);
	if(Debug())
	  {
	    Debug("We found a gamma event! Hooray!!");
	    ctr++;
	  }
	siblingsS.clear();
	siblingsT.clear();
	sibShwrFin.clear();
      } // <-- end s loop
    if(ctr > 0)
      {gamma_evtctr++;}
    if(ctr == 0)
      {null_evtctr++;}
    return true;
  } // <__ end Reconstruct

  void ERAlgoNGamma::ProcessEnd(TFile* fout)
  {
    if(fout)
      {
	fout->cd();
	if(_alg_tree) {_alg_tree->Write();}
	if(_empart_tree) {_empart_tree->Write();}
      }
    return;
  }

  void ERAlgoNGamma::ClearTree()
  {
    _E = -1;
    _thatE = -1;
    _PDG = -1;
    _dEdx = -1;
    _VsTrack = -1;
    _IP = -1;
    _IPthisStart = -1;
    _IPthatStart = -1;
    _IPtrkBody = -1;
    
  }

}

#endif
