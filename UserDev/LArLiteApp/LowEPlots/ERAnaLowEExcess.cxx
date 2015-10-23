#ifndef ERTOOL_ERANALOWEEXCESS_CXX
#define ERTOOL_ERANALOWEEXCESS_CXX

#include "ERAnaLowEExcess.h"

namespace ertool {

ERAnaLowEExcess::ERAnaLowEExcess(const std::string& name)
	: AnaBase(name)
	, _result_tree(nullptr)
{

	PrepareTreeVariables();

	TPC.Min(0 + 10,
	        -(::larutil::Geometry::GetME()->DetHalfHeight()) + 10,
	        0 + 10);

	TPC.Max(2 * (::larutil::Geometry::GetME()->DetHalfWidth()) - 10,
	        ::larutil::Geometry::GetME()->DetHalfHeight() - 10,
	        ::larutil::Geometry::GetME()->DetLength() - 10);

	// set default energy cut (for counting) to 0
	_eCut = 0;


}


bool ERAnaLowEExcess::Analyze(const EventData &data, const ParticleGraph &graph)
{
	_result_tree->SetName(Form("%s", _treename.c_str()));

	// Reset tree variables
	// Assume we will mis-ID
	ResetTreeVariables();

	_numEvts += 1;

	/// This variable seems to indicate whether a neutrino was reconstructed
	/// (IE a "ccsingleE" was found)
	bool reco = false;

	// size of ParticleSet should be the number of neutrinos found, each associated with a single electron
	auto const& particles = graph.GetParticleArray();

	// First off, if no nue was reconstructed, skip this event entirely.
	for ( auto const & p : particles )
		if ( abs(p.PdgCode()) == 12 ) reco = true;
	if (!reco) return false;

	// Reset the particleID object representing the single electron found
	// singleE_particleID.Reset();
	// Reset the ertool::Shower copy of the ccsingleE-identified ertool::Shower
	singleE_shower.Reset();

	// Loop over particles and find the nue
	for ( auto const & p : particles ) {
		if ( abs(p.PdgCode()) == 12 ) {
			// Save the neutrino vertex to the ana tree
			_x_vtx = p.Vertex().at(0);
			_y_vtx = p.Vertex().at(1);
			_z_vtx = p.Vertex().at(2);

			// Save whether the neutrino verted was inside of fiducial volume
			if (!(TPC.Contain(p.Vertex())))
				_is_fiducial = false;
			else _is_fiducial = true;

			// get all descendants of the neutrino in order to calculate total energy deposited
			_e_dep = 0;
			auto const descendants = graph.GetAllDescendantNodes(p.ID());
			for ( auto const & desc : descendants) {
				auto const & part = graph.GetParticle(desc);
				// does this particle have a reco ID?
				if (part.HasRecoObject() == true) {
					// get the reco object's dep. energy
					// if shower
					if (part.RecoType() == kShower) {
						_e_dep += data.Shower(part.RecoID())._energy;
					}
					if (part.RecoType() == kTrack) {
						_e_dep += data.Track(part.RecoID())._energy;
					}
				}// if the particle has a reco object
			}// for all neutrino descendants

			// Compute the neutrino energy
			_e_nuReco = 0;
			//find the neutrino daughter that is a lepton
			for (auto const& d : p.Children()) {

				auto daught = graph.GetParticle(d);
				// std::cout<<"\tneutrino daughter PDG = "<<daught.PdgCode()<<std::endl;
				// This is the "ccsinglee" electron. Store it's "particleID" to find it in mcparticlegraph later
				if (daught.PdgCode() == 11) {
					// singleE_particleID = ertool_helper::ParticleID(daught.PdgCode(),
					// 	daught.Vertex(),
					// 	daught.Momentum());
					// std::cout<<"Made singleE_particleID with vertex "<<daught.Vertex()<<std::endl;
					singleE_shower = data.Shower(daught.RecoID());
				}

				_e_nuReco += daught.KineticEnergy();
				if (daught.HasRecoObject() == true) {
					// get the reco object's dep. energy
					if (daught.RecoType() == kTrack) {
						auto mytrack = data.Track(daught.RecoID());
						double current_tracklen = ( mytrack.back() - mytrack.front() ).Length();
						if (current_tracklen > _longestTrackLen) _longestTrackLen = current_tracklen;
					}
				}// if the particle has a reco object
			} // End loop over neutrino children
		}// if we found the neutrino
	}// End loop over particles


	// Get MC particle set
	auto const& mc_graph = MCParticleGraph();
	for ( auto const & mc : mc_graph.GetParticleArray() ) {
		// Find the shower particle in the mcparticlegraph that matches the object CCSingleE identified
		// as the single electron (note, the mcparticlegraph object could be a gamma, for example)
		// To do this, grab the ertool Shower in event_data associated with each mcparticlegraph
		// shower particle and compare
		// (note this works for perfect-reco, but there needs more sophisticated methods for reco-reco)
		if (mc.RecoType() == kShower) {
			ertool::Shower mc_ertoolshower = data.Shower(mc);
			// We match ertool showers from mc particle graph to ertool showers from reco particle graphs
			// By comparing the energy to double precision... can consider also comparing _dedx and _time as well
			if (mc_ertoolshower._energy == singleE_shower._energy) {
				auto parent = mc_graph.GetParticle(mc.Parent());
				_parentPDG = parent.PdgCode();
				_mcPDG = mc.PdgCode();

				// if (abs(_parentPDG) == 11) {
				// 	std::cout << "Energy of particle tagged is " << mc_ertoolshower._energy << std::endl;
				// 	std::cout << "PDG of particle tagged is " << _mcPDG << std::endl;
				// 	// std::cout << mc_graph.Diagram() << std::endl;
				// }
			}
		}


		/// This stuff takes the truth neutrino information and fills flux_reweight-relevant
		/// branches in the ttree (used later on to weight events in final stacked histograms)
		if (abs(mc.PdgCode()) == 12 || abs(mc.PdgCode()) == 14 ) {

			int ntype = 0;
			int ptype = 0;
			double E = mc.Energy() / 1e3;

			//	std::cout << E << std::endl;

			if (mc.PdgCode() == 12)       ntype = 1;
			else if (mc.PdgCode() == -12) ntype = 2;
			else if (mc.PdgCode() ==  14) ntype = 3;
			else if (mc.PdgCode() == -14) ntype = 4;

			if (mc.ProcessType() == ::ertool::kK0L) ptype = 3;
			else if (mc.ProcessType() == ::ertool::kKCharged) ptype = 4;
			else if (mc.ProcessType() == ::ertool::kMuDecay) ptype = 1;
			else if (mc.ProcessType() == ::ertool::kPionDecay) ptype = 2;

			if (mc.ProcessType() != ::ertool::kK0L &&
			        mc.ProcessType() != ::ertool::kKCharged &&
			        mc.ProcessType() != ::ertool::kMuDecay &&
			        mc.ProcessType() != ::ertool::kPionDecay) {

				std::cout << " PDG : " << mc.PdgCode() << " Process Type : " << mc.ProcessType() << " from " <<
				          ::ertool::kK0L <<  " or " <<
				          ::ertool::kKCharged << " or " <<
				          ::ertool::kMuDecay << " or " <<
				          ::ertool::kPionDecay << std::endl;
			}

			_weight = _fluxRW.get_weight(E, ntype, ptype);

			break;

		}
	}

	_result_tree->Fill();

	return true;
}

void ERAnaLowEExcess::ProcessEnd(TFile* fout)
{

	if (fout) {
		fout->cd();
		_result_tree->Write();
	}

	return;

}

void ERAnaLowEExcess::PrepareTreeVariables() {

	if (_result_tree) { delete _result_tree; }

	_result_tree = new TTree(Form("%s", _treename.c_str()), "Result Tree");
	_result_tree->Branch("_numEvts", &_numEvts, "numEvts/I");
	_result_tree->Branch("_is_fiducial", &_is_fiducial, "is_fiducial/O");
	_result_tree->Branch("_e_nuReco", &_e_nuReco, "e_nuReco/D");
	_result_tree->Branch("_e_dep", &_e_dep, "e_dep/D");
	_result_tree->Branch("_weight", &_weight, "weight/D");
	_result_tree->Branch("_parentPDG", &_parentPDG, "parent_PDG/I");
	_result_tree->Branch("_mcPDG", &_mcPDG, "mc_PDG/I");
	_result_tree->Branch("_longestTrackLen", &_longestTrackLen, "longest_tracklen/D");
	_result_tree->Branch("_x_vtx", &_x_vtx, "x_vtx/D");
	_result_tree->Branch("_y_vtx", &_y_vtx, "y_vtx/D");
	_result_tree->Branch("_z_vtx", &_z_vtx, "z_vtx/D");


	return;
}

void ERAnaLowEExcess::ResetTreeVariables() {

	_numEvts = 0;
	_is_fiducial = false;
	_e_nuReco = 0;
	_e_dep = 0;
	_parentPDG = -99999;
	_mcPDG = -99999;
	_longestTrackLen = 0;
	_x_vtx = -999.;
	_y_vtx = -999.;
	_z_vtx = -999.;

	return;

}


}

#endif
