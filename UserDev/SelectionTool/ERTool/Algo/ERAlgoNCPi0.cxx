#ifndef ERTOOL_ERALGONCPI0_CXX
#define ERTOOL_ERALGONCPI0_CXX

#include "ERAlgoNCPi0.h"

namespace ertool {

	ERAlgoNCPi0::ERAlgoNCPi0(const std::string& name) 
	: AlgoBase(name)
	, _pi0_tree(nullptr)
	, _pi0_selected_tree(nullptr)
	, CandTree(nullptr)
	{}

	void ERAlgoNCPi0::Reset()
	{}

	void ERAlgoNCPi0::AcceptPSet(const ::fcllite::PSet& cfg)
	{}

	void ERAlgoNCPi0::ProcessBegin()
	{
    // allpi0 Tree
		delete _pi0_tree;
		_pi0_tree = new TTree("_pi0_tree","Pi0 Tree");
		_pi0_tree->Branch("_Rmass",&_Rmass,"mass/D");
		_pi0_tree->Branch("_RE_A",&_RE_A,"E_A/D");
		_pi0_tree->Branch("_RE_B",&_RE_B,"E_B/D");
		_pi0_tree->Branch("_RIP",&_RIP,"IP/D");
		_pi0_tree->Branch("_ROangle",&_ROangle,"Oangle/D");
		_pi0_tree->Branch("_Rnshowers",&_Rnshowers,"nshowers/D");
		_pi0_tree->Branch("_Anshowers",&_Anshowers,"Anshowers/D");
		_pi0_tree->Branch("_Rvtx_X",&_Rvtx_X,"vtx_X/D");
		_pi0_tree->Branch("_Rvtx_Y",&_Rvtx_Y,"vtx_Y/D");
		_pi0_tree->Branch("_Rvtx_Z",&_Rvtx_Z,"vtx_Z/D");
    // selected / candidate pi0 Tree
		delete _pi0_selected_tree;
		_pi0_selected_tree = new TTree("_pi0_selected_tree","Pi0 selected Tree");
		_pi0_selected_tree->Branch("_Rmass_S",&_Rmass_S,"mass_S/D");
		_pi0_selected_tree->Branch("_RE_A_S",&_RE_A_S,"E_A_S/D");
		_pi0_selected_tree->Branch("_RE_B_S",&_RE_B_S,"E_B_S/D");
		_pi0_selected_tree->Branch("_RIP_S",&_RIP_S,"IP_S/D");
		_pi0_selected_tree->Branch("_ROangle_S",&_ROangle_S,"Oangle_S/D");
		_pi0_selected_tree->Branch("_Rvtx_X_S",&_Rvtx_X_S,"vtx_X_S/D");
		_pi0_selected_tree->Branch("_Rvtx_Y_S",&_Rvtx_Y_S,"vtx_Y_S/D");
		_pi0_selected_tree->Branch("_Rvtx_Z_S",&_Rvtx_Z_S,"vtx_Z_S/D");
		_pi0_selected_tree->Branch("_RadL_A",&_RadL_A,"RadL_A_S/D");
		_pi0_selected_tree->Branch("_RadL_B",&_RadL_B,"RadL_B_S/D");
   // Candidate tree
		CandTree = new TTree("CandTree","CandTree");
		CandTree->Branch("Rnshowers",&_Rnshowers,"Rnshowers/I");
		CandTree->Branch("Candsize",&_Candsize,"Candsize/I");
//		CandTree->Branch("EDet_B",&_EDet_B,"EDet_B/D");
                CandTree->Branch("mass_C",&_mass_C,"mass_C/D");
		CandTree->Branch("C_X",&_C_X,"C_X/D");
		CandTree->Branch("C_Y",&_C_Y,"C_Y/D");
		CandTree->Branch("C_Z",&_C_Z,"C_Z/D");
		CandTree->Branch("CE_A",&_CE_A,"CE_A/D");
		CandTree->Branch("CE_B",&_CE_B,"CE_B/D");
		CandTree->Branch("COangle",&_COangle,"COangle/D");
		CandTree->Branch("CIP",&_CIP,"CIP/D");
		CandTree->Branch("CVxtDist_A",&_CVxtDist_A,"CVxtDist_A/D");
		CandTree->Branch("CVxtDist_B",&_CVxtDist_B,"CVxtDist_B/D");
		CandTree->Branch("CVDistToEdge_A",& _CVDistToEdge_A,"CVDistToEdge_A/D");
		CandTree->Branch("CVDistToEdge_B",& _CVDistToEdge_B,"CVDistToEdge_B/D");
		CandTree->Branch("CSDistToEdge_A",& _CSDistToEdge_A,"CSDistToEdge_A/D");
		CandTree->Branch("CSDistToEdge_B",& _CSDistToEdge_B,"CSDistToEdge_B/D");
	}

	bool ERAlgoNCPi0::Reconstruct(const EventData &data, ParticleGraph& graph)
	{
		auto DetW = larutil::Geometry::GetME()->DetHalfWidth();
		auto DetH = larutil::Geometry::GetME()->DetHalfHeight();
		auto DetL = larutil::Geometry::GetME()->DetLength();
	// Algoo that returns a pi0 if there are ONLY two good corelated showers
	// This would not depend the selection on dEdx
	::geoalgo::AABox  volume(0,-DetH,0,2.*DetW,DetH,DetL);// fill in with detector properties

	// Storage of the two ID's that pass
        std::vector<std::pair<NodeID_t,NodeID_t>> CandidatePairs;
        
	// First check to see if there are atleast two showers in the event 
        if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;
        auto datacpy = data;
		// Get all the shower ID's 
        auto const& shrIDs = graph.GetParticleNodes(RecoType_t::kShower);
        _Rnshowers = shrIDs.size();
	// quick loop to check the number of avaliable showers to use 
        _Anshowers = 0;
        for (auto const& shrID1 : shrIDs){
	        // If they are cosmic showers we can't use them
        	if(graph.GetParticle(shrID1).ProcessType()==kCosmic) continue;
		// If they are showers from showers 
        	if(graph.GetParticle(shrID1).Descendant()==1) continue;
        	_Anshowers++;	
        }

		// Loop over Pairs of showers
        for (auto const& shrID1 : shrIDs){
			// Make the shower 1
        	auto const& shr1 = datacpy.Shower(graph.GetParticle(shrID1).RecoID());
			// Avoid cosmics and already acounted for showers
        	if(graph.GetParticle(shrID1).ProcessType()==kCosmic) continue;
        	if(graph.GetParticle(shrID1).Descendant()==1) continue;

        	for (auto const& shrID2 : shrIDs){
        		if (shrID2 <= shrID1) continue;
			// Make the shower 2
        		auto const& shr2 = datacpy.Shower(graph.GetParticle(shrID2).RecoID());
        		if(graph.GetParticle(shrID2).ProcessType()==kCosmic) continue;
        		if(graph.GetParticle(shrID2).Descendant()==1) continue;

        		geoalgo::Vector_t dflipA(-1.*shr1.Dir()[0],-1.*shr1.Dir()[1],-1.*shr1.Dir()[2]);
        		geoalgo::Vector_t dflipB(-1.*shr2.Dir()[0],-1.*shr2.Dir()[1],-1.*shr2.Dir()[2]);

			// Check it if is NAN
        		if(dflipA[0]!=dflipA[0] ){ if(sqrt(pow(dflipA[0],2))>0.){std::cout<<"safe"<<std::endl;} else continue;}
        		if(dflipB[0]!=dflipB[0] ){ if(sqrt(pow(dflipB[0],2))>0.){std::cout<<"safe"<<std::endl;} else continue;}
			// Make the backwards projection for the showers
        		auto bksa = ::geoalgo::HalfLine_t(shr1.Start(),dflipA);
        		auto bksb =::geoalgo::HalfLine_t(shr2.Start(),dflipB);
			// Calc the Opening angle of the showers
        		double Oangle = shr1.Dir().Angle(shr2.Dir());
			geoalgo::Point_t vertex(3);// need to fill out 
			// Calc the vertex point of the two showers. the true designated backwards project
			auto dot = _geoAlgo.commonOrigin(shr1, shr2, vertex, true);
			// Calc Dist from Vertex to start of shower
			_RadL_A = vertex.Dist(shr1.Start());
			_RadL_B = vertex.Dist(shr2.Start());
			// Calc Diretion of two correlated shower
			geoalgo::Vector_t momentum(3);// need to fill out
			double e1 = shr1._energy;
			double e2 = shr2._energy;
			geoalgo::Vector_t mom_vect(shr2.Dir()*e1 +shr1.Dir()*e2) ;
			mom_vect.Normalize();
			double mom2 = sqrt(pow(shr1._energy,2)+pow(shr2._energy,2)+2*shr2._energy*shr1._energy*cos(Oangle));
			momentum = mom_vect*mom2;

			// Implement a logical flip based on vtx and radL and start points
			// Need to copy over code, At the moment nothing is done
				//===========================================
				//$$ FIll _pi0 tree first for getting info 
				//===========================================
			_RE_A = shr1._energy;
			_RE_B = shr2._energy;
			_RIP = pow(_geoAlgo.SqDist(bksa,bksb),0.5);
			_ROangle = Oangle;
				_Rmass = sqrt(2 * shr1._energy * shr2._energy *(1.-cos(Oangle)));// Old Mass 
				_Rvtx_X = vertex[0];
				_Rvtx_Y = vertex[1];
				_Rvtx_Z = vertex[2];

				_pi0_tree->Fill();
			//===========================================
			// Make cuts based on relation of two showers
			//===========================================

			//+*+*+*+*  place cuts on first shower
				if(shr1._energy<_energy_min) continue;
			//####################################
			//+*+*+*+* place cuts on second shower
				if(shr2._energy<_energy_min) continue;
			//####################################

			//+*+*+*+*  place cuts on first shower
				if(shr1._energy>_energy_max) continue;
			//####################################
			//+*+*+*+* place cuts on second shower
				if(shr2._energy>_energy_max) continue;
			//####################################

			//+*+*+*+*Cut on E1+E2 Min
				if(shr1._energy+shr2._energy<_energy_sum_min) continue;
			//if(shr1._energy+shr2._energy<_energy_sum_min){std::cout<<" Break A"<<std::endl; continue;}
			//####################################
			//+*+*+*+*Cut on E1+E2 Max
				if(shr1._energy+shr2._energy>_energy_sum_max) continue;
			//if(shr1._energy+shr2._energy>_energy_sum_max){std::cout<<" Break B"<<std::endl; continue;}
			//####################################

			//+*+*+*+*Cut on (E1-E2)/(E1+E2) Min
				if(fabs((shr1._energy-shr2._energy))/(shr1._energy+shr2._energy) <_energy_asy_min) continue;
			//####################################
			//+*+*+*+*Cut on (E1-E2)/(E1+E2) Max
				if(fabs((shr1._energy-shr2._energy))/(shr1._energy+shr2._energy) >_energy_asy_max) continue;
			//####################################

			//+*+*+*+*Cut on min 3d opening angle
				if(Oangle< _OpeningAnglemin) continue;
			//####################################
			//+*+*+*+*Cut on max 3d opening angle
				if(Oangle> _OpeningAnglemax) continue;
			//####################################

			//+*+*+*+* Cut on the min IP
				if(pow(_geoAlgo.SqDist(bksa,bksb),0.5)<_IPmin) continue;
			//####################################
			//+*+*+*+* Cut on the max IP
				if(pow(_geoAlgo.SqDist(bksa,bksb),0.5)>_IPmax) continue;
			//####################################

			// For shower_A
			//+*+*+*+*Cut on RadLength (Dist from vtx to Start showers) 
				if(_RadL_A< _RadL_min) continue;
			//####################################
			//+*+*+*+*Cut on max 3d opening angle
				if(_RadL_A> _RadL_max) continue;
			//####################################
			// For shower_B
			//+*+*+*+*Cut on RadLength (Dist from vtx to Start showers) 
				if(_RadL_B< _RadL_min) continue;
			//####################################
			//+*+*+*+*Cut on max 3d opening angle
				if(_RadL_B> _RadL_max) continue;
			//####################################

			//+*+*+*+*Cut on fiducial X position of vertex) 
				if(_Rvtx_X > 2*DetW -_fidx_max ||_Rvtx_X < 0.+_fidx_min) continue;
			//####################################
			//+*+*+*+*Cut on fiducial Y position of vertex) 
				if(_Rvtx_Y > DetH - _fidy_max ||_Rvtx_Y < -DetH + _fidy_min) continue;
			//####################################
			//+*+*+*+*Cut on fiducial Z position of vertex) 
				if(_Rvtx_Z > DetL-_fidz_max ||_Rvtx_Z < 0.+ _fidz_min) continue;
			//####################################

			//+*+*+*+*Cut on fiducial Z position of vertex) 
				if(momentum[0]/momentum.Length() >_PDX_max ||momentum[0]/momentum.Length() <_PDX_min) continue;
				if(momentum[1]/momentum.Length() >_PDY_max ||momentum[1]/momentum.Length() <_PDY_min) continue;
				if(momentum[2]/momentum.Length() >_PDZ_max ||momentum[2]/momentum.Length() <_PDZ_min) continue;
			//####################################

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Only important if running cosmic algo's prior to this algo.


			//+*+*+*+*Cut on assosiated tracks at the vertex 
			// loop over all cosmics tracks.
			// first make a sphere of the vertex 
			// want to ensure that the vertex is not near a cosmic track
			// Vertex of the pi0 point
				bool cosmicass= false;
				geoalgo::Point_t pi0vpt(vertex[0],vertex[1],vertex[2]);
			// hard code for now
			// This should not be seen if no previous cosmic algo's 
				double _vtxsphere_radius = 5;
				auto vtxsphere = ::geoalgo::Sphere(pi0vpt, _vtxsphere_radius);
				auto const& trkIDs = graph.GetParticleNodes(RecoType_t::kTrack);
				for (auto const& tk : trkIDs){
					if(graph.GetParticle(tk).ProcessType()!=kCosmic) continue;
					auto const& tk1 = datacpy.Track(graph.GetParticle(tk).RecoID());
					geoalgo::Point_t trk_start(tk1.at(0)[0],tk1.at(0)[1],tk1.at(0)[2]);
					geoalgo::Point_t trk_end(tk1.at(tk1.size()-1)[0],tk1.at(tk1.size()-1)[1],tk1.at(tk1.size()-1)[2]);
					if(vtxsphere.Contain(trk_start) ||vtxsphere.Contain(trk_end)) cosmicass=true;
			}// for loop on tracks
			if(cosmicass) continue;
				//+*+*+*+*Cut on assosiated tracks at the vertex 
			// try with long cc
			bool ccass= false;
			// hard code needs to fix
			double _tLencut = 50;
			for (auto const& tk : trkIDs){
				if(graph.GetParticle(tk).ProcessType()==kCosmic) continue;
				auto const& tk1 = datacpy.Track(graph.GetParticle(tk).RecoID());
			// if the track is longer than 50
				geoalgo::Point_t trk_start(tk1.at(0)[0],tk1.at(0)[1],tk1.at(0)[2]);
				geoalgo::Point_t trk_end(tk1.at(tk1.size()-1)[0],tk1.at(tk1.size()-1)[1],tk1.at(tk1.size()-1)[2]);
				if(vtxsphere.Contain(trk_start) && tk1.Length()>_tLencut)  ccass=true;
				if(vtxsphere.Contain(trk_end) && tk1.Length()>_tLencut)  ccass=true;
		}// for loop on tracks
		
		if(ccass) continue;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// if there is a cosmic track inside the sphere get rid of the event
				//===========================================
				// If we make it to here we have passed 
				// all the cuts. Fill them into the ID vector 
				//===========================================
				// cut to time 
		std::pair<NodeID_t,NodeID_t> PassPair(shrID1,shrID2);
		CandidatePairs.push_back(PassPair);
			}// shower ID 2 
		}// shower ID 1 

	// If the size of the Candidate Pairs are 1 then we can to make a pi0 
		if( CandidatePairs.size() == 1) { 	
          // specify info for two selected gamma's
			auto const& cshr1 = datacpy.Shower(graph.GetParticle(CandidatePairs[0].first).RecoID());
			auto const& cshr2 = datacpy.Shower(graph.GetParticle(CandidatePairs[0].second).RecoID());
	// Put together variables for the tree.
			geoalgo::Vector_t cdflipA(-1.*cshr1.Dir()[0],-1.*cshr1.Dir()[1],-1.*cshr1.Dir()[2]);
			geoalgo::Vector_t cdflipB(-1.*cshr2.Dir()[0],-1.*cshr2.Dir()[1],-1.*cshr2.Dir()[2]);
			auto cbksa = ::geoalgo::HalfLine_t(cshr1.Start(),cdflipA);
			auto cbksb =::geoalgo::HalfLine_t(cshr2.Start(),cdflipB);
			double cOangle = cshr2.Dir().Angle(cshr1.Dir());
          // specify info for 1nd gamma
			auto const& gamma1vtx = cshr1.Start();
			auto const& gamma1mom = cshr1.Dir()*cshr1._energy;
			graph.GetParticle(CandidatePairs[0].first).SetParticleInfo(22,0,gamma1vtx,gamma1mom);
          // specify info for 2nd gamma
			auto const& gamma2vtx = cshr2.Start();
			auto const& gamma2mom = cshr2.Dir()*cshr2._energy;
			graph.GetParticle(CandidatePairs[0].second).SetParticleInfo(22,0,gamma2vtx,gamma2mom);
	// Compute mass
		double mass = sqrt(2 * cshr1._energy * cshr2._energy *(1.-cos(cOangle)));// Old Mass 
		//std::cout<<"Mass from size =1 "<< mass<<std::endl;
	// make the pi0
		Particle& pi0 = graph.CreateParticle();
		geoalgo::Point_t vertex(3);// need to fill out 
		auto dot = _geoAlgo.commonOrigin(cshr1, cshr2, vertex, true);
		geoalgo::Vector_t momentum(3);// need to fill out
	// Make the momentum from the photon Dirs	
		double e1 = cshr1._energy;
		double e2 = cshr2._energy;
		geoalgo::Vector_t mom_vect(cshr2.Dir()*e1 +cshr1.Dir()*e2) ;
		mom_vect.Normalize();
		double mom2 = sqrt(pow(cshr1._energy,2)+pow(cshr2._energy,2)+2*cshr2._energy*cshr1._energy*cos(cOangle));
		momentum = mom_vect*mom2;

		pi0.SetParticleInfo(111,mass,vertex,momentum);
		graph.SetParentage(pi0.ID(),graph.GetParticle(CandidatePairs[0].first).ID());
		graph.SetParentage(pi0.ID(),graph.GetParticle(CandidatePairs[0].second).ID());
		graph.SetSiblings(graph.GetParticle(CandidatePairs[0].first).ID(),graph.GetParticle(CandidatePairs[0].second).ID());
//	std::cout<<graph.Diagram()<<std::endl;
				//===========================================
				//$$ FIll _pi0_selected tree first for getting info 
				//===========================================
		_Rmass_S= mass;
		_RE_A_S = cshr1._energy;
		_RE_B_S = cshr2._energy;
		_RIP_S = pow(_geoAlgo.SqDist(cbksa,cbksb),0.5);
		_ROangle_S = cOangle;
		_Rvtx_X_S = vertex[0];
		_Rvtx_Y_S = vertex[1];
		_Rvtx_Z_S = vertex[2];
		_pi0_selected_tree->Fill();
				//===========================================
				//===========================================

	}// if candidpairs ==1




	// This has nothing to do with the actual code... it just is useful for looking at what passes
	// This section is not commeneted but it is the same as going through the first loop.
	// It is it's own loop becuase I wanted to fill in the size of the cand's in each entry. 
	// This just makes this slower but for not it is fine....
	//What do the Candidates look like 
		// Add it as debug mode
	_Candsize = 0;
	_Candsize = CandidatePairs.size();
        // loop over contained
	for(unsigned int c = 0; c<CandidatePairs.size(); c++){
		auto const& Cshr1 = datacpy.Shower(graph.GetParticle(CandidatePairs[c].first).RecoID());
		auto const& Cshr2 = datacpy.Shower(graph.GetParticle(CandidatePairs[c].second).RecoID());
		_CE_A = Cshr1._energy;
		_CE_B = Cshr2._energy;
		_COangle = Cshr1.Dir().Angle(Cshr2.Dir());
		geoalgo::Vector_t dflipA(-1.*Cshr1.Dir()[0],-1.*Cshr1.Dir()[1],-1.*Cshr1.Dir()[2]);
		geoalgo::Vector_t dflipB(-1.*Cshr2.Dir()[0],-1.*Cshr2.Dir()[1],-1.*Cshr2.Dir()[2]);
		auto bksa = ::geoalgo::HalfLine_t(Cshr1.Start(),dflipA);
		auto bksb =::geoalgo::HalfLine_t(Cshr2.Start(),dflipB);
		geoalgo::Vector_t fflipA(Cshr1.Dir()[0],Cshr1.Dir()[1],Cshr1.Dir()[2]);
		geoalgo::Vector_t fflipB(Cshr2.Dir()[0],Cshr2.Dir()[1],Cshr2.Dir()[2]);
		auto fksa = ::geoalgo::HalfLine_t(Cshr1.Start(),dflipA);
		auto fksb =::geoalgo::HalfLine_t(Cshr2.Start(),dflipB);
		_CIP = pow(_geoAlgo.SqDist(bksa,bksb),0.5);
                        geoalgo::Point_t vertex(3);// need to fill out 
                        auto dot = _geoAlgo.commonOrigin(fksa,fksb , vertex,true );// Needs to be a shr geo obj
                        _CVxtDist_A = vertex.Dist(Cshr1.Start());
                        _CVxtDist_B = vertex.Dist(Cshr2.Start());
                        auto ExitingPoint_A = _geoAlgo.Intersection(volume, fksa);
                        auto ExitingPoint_B = _geoAlgo.Intersection(volume, fksb);
                        auto VtxExitPoint_A = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(vertex,fflipA));
                        auto VtxExitPoint_B = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(vertex,fflipA));
                        bool vtct = volume.Contain(vertex);
                        bool shAct = volume.Contain(Cshr1.Start());
                        bool shBct = volume.Contain(Cshr2.Start());
                        if(!vtct) continue;
                        if(!shAct) continue;
                        if(!shBct) continue;
                        geoalgo::Point_t ShrStart_A(Cshr1.Start()[0],Cshr1.Start()[1],Cshr1.Start()[2]);
                        geoalgo::Point_t ShrStart_B(Cshr2.Start()[0],Cshr2.Start()[1],Cshr2.Start()[2]);
                        _CVDistToEdge_A = vertex.Dist(ExitingPoint_A[0]);
                        _CSDistToEdge_A = ShrStart_A.Dist(VtxExitPoint_A[0]);
                        _CVDistToEdge_B = vertex.Dist(ExitingPoint_B[0]);
                        _CSDistToEdge_B = ShrStart_B.Dist(VtxExitPoint_B[0]);

                        _C_X = vertex[0];
                        _C_Y = vertex[1];
                        _C_Z = vertex[2];
			_mass_C = sqrt(2*_CE_A*_CE_B*(1-cos(_COangle)));
                        CandTree->Fill();
                }// loop over over CandidatePairs



                return true;}

                void ERAlgoNCPi0::ProcessEnd(TFile* fout)
                {
                	if(fout) {
                		fout->cd();
                		_pi0_tree->Write();
                		_pi0_selected_tree->Write();
                		CandTree->Write();
		}//fout
	}


}

#endif
