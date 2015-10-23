#ifndef ERTOOL_ERALGOPI0_CXX
#define ERTOOL_ERALGOPI0_CXX

#include "ERAlgopi0.h"

namespace ertool {

  ERAlgopi0::ERAlgopi0(const std::string& name)
   : AlgoBase(name)
    , _ll_tree(nullptr)
  {
	}

  void ERAlgopi0::Reset()
  {}

  void ERAlgopi0::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    // Load EMPart params
    _alg_emp.AcceptPSet(cfg);
	}

  void ERAlgopi0::ProcessBegin()
  {
    // Initialize LL Tree
    delete _ll_tree;
    _ll_tree = new TTree("_ll_tree","LL Tree");
    _ll_tree->Branch("_dedx_A",&_dedx_A,"dedx_A/D");
    _ll_tree->Branch("_dedx_B",&_dedx_B,"dedx_B/D");
    _ll_tree->Branch("_vtxDist_A",&_vtxDist_A,"vtxDist_A/D");
    _ll_tree->Branch("_vtxDist_B",&_vtxDist_B,"vtxDist_B/D");
    _ll_tree->Branch("_vtx_IP",&_vtx_IP,"vtx_IP/D");
    _ll_tree->Branch("_mass",&_mass,"mass/D");
    _ll_tree->Branch("_angle",&_angle,"angle/D");
    _ll_tree->Branch("_E_A",&_E_A,"E_A/D");
    _ll_tree->Branch("_E_B",&_E_B,"E_B/D");

    _ll_tree->Branch("_ll_photon_A",&_ll_photon_A,"ll_photon_A/D");
    _ll_tree->Branch("_ll_photon_B",&_ll_photon_B,"ll_photon_B/D");
	}

  bool ERAlgopi0::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
	// First check to see if there are atleast two showers in the event 
	if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;

	auto datacpy = data;
	auto const& shrIDs = graph.GetParticleNodes(RecoType_t::kShower);
 
	morepi0s:
	//best LL for pi0 
	double bestpi0ll = -1e10;
	// Name the shower ID's 
	NodeID_t BestShr1;
	NodeID_t BestShr2;
	geoalgo::Point_t vertex(3);
	geoalgo::Vector_t momentum(3);

	for (auto const& shrID1 : shrIDs){
		for (auto const& shrID2 : shrIDs){
			if (shrID2 <= shrID1) continue;
		// make sure that they are not already used 
		if ( graph.GetParticle(shrID1).RelationAssessed()|| graph.GetParticle(shrID2).RelationAssessed() ) continue;
		// make a const copy for this run
		auto const& shr1 = datacpy.Shower(graph.GetParticle(shrID1).RecoID());
		auto const& shr2 = datacpy.Shower(graph.GetParticle(shrID2).RecoID());
		
		double likelihood = 0.;
		double mass       = -1.;
		_angle = 0;
		// Calculate the LL
		LL(shr1, shr2, likelihood, mass, vertex, momentum);

		// Store the LL Value;
		// check if the ll is better... if it is... then save the shower info and ll
		if(bestpi0ll<likelihood){
		BestShr1 = shrID1;
		BestShr2 = shrID2;
		bestpi0ll = likelihood;
		}

						}// shr2id 
					}// shr1id 

		// We have a good pi0 let's make a particle 
		if(bestpi0ll!= -1e10 ){
		//if(bestpi0ll!= -1e10  && Above some llcut){
			// Set the best 
			double likelihood = 0.;
			double mass       = -1.;
			_angle = 0;
	// Store the LL Value;
	// check if the ll is better... if it is... then save the shower info and ll
        auto const& bshr1 = datacpy.Shower(graph.GetParticle(BestShr1).RecoID());
        auto const& bshr2 = datacpy.Shower(graph.GetParticle(BestShr2).RecoID());
        LL(bshr1, bshr2, likelihood, mass, vertex, momentum);
	// We want to check a few things first... 
	// STILL NEED TO CHECK A FEW THINGS>>> ADD LIKELIHOODCUTS

          // specify info for 1st gamma
          auto const& gamma1vtx = bshr1.Start();
          auto const& gamma1mom = bshr1.Dir()*bshr1._energy;
          graph.GetParticle(BestShr1).SetParticleInfo(22,0,gamma1vtx,gamma1mom);
          // specify info for 2nd gamma
          auto const& gamma2vtx = bshr2.Start();
          auto const& gamma2mom = bshr2.Dir()*bshr2._energy;
          graph.GetParticle(BestShr2).SetParticleInfo(22,0,gamma2vtx,gamma2mom);
          // create pi0
          Particle& pi0 = graph.CreateParticle();

          // Get the the IP???RG  DC will be adding the function that is capible with showers.
          pi0.SetParticleInfo(111,mass,vertex,momentum);
          //pi0.SetParticleInfo(111,mass,vertex,momentum);// score based on vertex IP? or LL
          // Add relationships
          graph.SetParentage(pi0.ID(),BestShr1);
          graph.SetParentage(pi0.ID(),BestShr2);
          graph.SetSiblings(BestShr1,BestShr2);

	// DAM using a go to... 
	goto morepi0s;
	}// If Bestpi0ll

return true;}

  void ERAlgopi0::ProcessEnd(TFile* fout)
  {
	if(fout) {
	fout->cd();
	_ll_tree->Write();
	}//
  }

//===========================
	// make a LL if shower combo is pi0 like
//===========================
  void ERAlgopi0::LL(const Shower& shower_a,
                   const Shower& shower_b,
                   double& ll,
                   double& mass,
                   geoalgo::Point_t& vtx,
                   geoalgo::Vector_t& mom)
		{
	// First define the LL to something we know
	ll   = -1.e-15;
	_mass = -1.;
	// Define each shower
	_E_A = shower_a._energy;
	_E_B = shower_b._energy;
	_dedx_A = shower_a._dedx;
	_dedx_B = shower_b._dedx;

	// make sure each shower energy is above 0
	if( (_E_A < 0) || (_E_B < 0) ) return;		
	if( (_E_A <  _energy_min) ||(_E_B <  _energy_min)||(_E_A >  _energy_max)||(_E_B >  _energy_max)) return;		
	
	// this gets angle and sets the vertex origin
	_dot = _geoAlgo.commonOrigin(shower_a, shower_b, vtx, true);
	
	// this is the distange from start of shower to vertex point
	_vtxDist_A = vtx.Dist(shower_a.Start());
	_vtxDist_B = vtx.Dist(shower_b.Start());

	_angle = shower_a.Dir().Angle(shower_b.Dir());
	
    // Corrected for the photon energy
    ::EMShowerProfile shower_prof;
    ::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);

    auto xs_a = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shower_a.Start(),shower_a.Dir()));
    auto xs_b = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shower_b.Start(),shower_b.Dir()));

    if(!xs_a.size() || !xs_b.size()) return;

    double energy_a = shower_a._energy / shower_prof.EnergyContainment( shower_a.Start().Dist(xs_a[0]) );
    double energy_b = shower_b._energy / shower_prof.EnergyContainment( shower_b.Start().Dist(xs_b[0]) );

    // Compute mass
    _mass = sqrt(4 * energy_a * energy_b * pow(sin(_angle/2.),2));
    mass = _mass;

    // calculate momentum:
    geoalgo::Vector_t energy(shower_a.Dir() * energy_a + shower_b.Dir() * energy_b);
    energy.Normalize();
    double mom2 = sqrt(pow(energy_a,2)+pow(energy_b,2)+2*energy_a*energy_b*cos(_angle));
    mom = energy*mom2;

    // get the shower dedx value
    _dedx_A = shower_a._dedx;
    _dedx_B = shower_b._dedx;

    // calculate IP 
    _vtx_IP = -999;
//	_ = _geoAlgo.commonOrigin(shower_a, shower_b, vtx, true);
//^^^^^^^^^^^^^^^^^
//NEEDS TO BE DONE

    // likelihood of shower to be a gamma:
    _ll_photon_A = _alg_emp.LL(false,shower_a._dedx,_vtxDist_A);
    _ll_photon_B = _alg_emp.LL(false,shower_b._dedx,_vtxDist_B);

    // sum up the likelihoods
    _ll += _ll_photon_A;
    _ll += _ll_photon_B;
    _ll = ll;

    _ll_tree->Fill();
		}// LL for pi0

}

#endif
