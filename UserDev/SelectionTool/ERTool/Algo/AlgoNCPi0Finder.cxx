#ifndef ERTOOL_ALGONCPI0FINDER_CXX
#define ERTOOL_ALGONCPI0FINDER_CXX

#include "AlgoNCPi0Finder.h"

namespace ertool {

	AlgoNCPi0Finder::AlgoNCPi0Finder(const std::string& name) : AlgoBase(name),
	_OPtree(nullptr),
	_tracktree(nullptr),
	_fulltracktree(nullptr),
	_bcontracktree(nullptr),
	_hcontracktree(nullptr),
	_ncontracktree(nullptr)
	{}

	void AlgoNCPi0Finder::Reset()
	{}

	void AlgoNCPi0Finder::AcceptPSet(const ::fcllite::PSet& cfg)
	{}

	void AlgoNCPi0Finder::ProcessBegin()
	{
		// Tree that looks at the OP tracks
		delete _OPtree;
		_OPtree = new TTree("_OPtree","OnePT Tree");
		_OPtree->Branch("_NExitingOPTracks",&_NExitingOPTracks,"NExitOPTracks/D");
		_OPtree->Branch("_NTracks_OPTracks",&_NTracks_OPTracks,"NTracksOPTracks/D");
		_OPtree->Branch("_Length_OPTracks",&_Length_OPTracks,"LengthOTracks/D");
		_OPtree->Branch("_SDistToTPC_OPTracks",&_SDistToTPC_OPTracks,"SDistToTPCOTracks/D");
		_OPtree->Branch("_EDistToTPC_OPTracks",&_EDistToTPC_OPTracks,"EDistToTPCOTracks/D");
		// Global variables 
		_OPtree->Branch("_pi0X",&_pi0X,"pi0X/D");
		_OPtree->Branch("_pi0Y",&_pi0Y,"pi0Y/D");
		_OPtree->Branch("_pi0Z",&_pi0Z,"pi0Z/D");
		
			
		delete _tracktree;
		_tracktree = new TTree("_tracktree","Track Tree");
		_tracktree->Branch("_LongestTrack",&_LongestTrack,"longesttrack/D");
		_tracktree->Branch("_SecondLongestTrack",&_SecondLongestTrack,"secondlongesttrack/D");
		delete _fulltracktree;
		_fulltracktree = new TTree("_fulltracktree","Full Track Tree");
		_fulltracktree->Branch("_fbtrackcount",&_btrackcount,"fbtrackcount/I");

		_fulltracktree->Branch("_fblongesttrack",&_blongesttrack,"fblongesttrack/D");
		_fulltracktree->Branch("_fhtrackcount",&_htrackcount,"fhtrackcount/I");
		_fulltracktree->Branch("_fhlongesttrack",&_hlongesttrack,"fhlongesttrack/D");
		_fulltracktree->Branch("_fntrackcount",&_ntrackcount,"fntrackcount/I");
		_fulltracktree->Branch("_fnlongesttrack",&_nlongesttrack,"fnlongesttrack/D");

		delete _bcontracktree;
		_bcontracktree = new TTree("_bcontracktree","Largest Track Tree");
		_bcontracktree->Branch("_btrackcount",&_btrackcount,"btrackcount/I");
		_bcontracktree->Branch("_blongesttrack",&_blongesttrack,"blongesttrack/D");
		delete _hcontracktree;
		_hcontracktree = new TTree("_hcontracktree","Largest Track Tree");
		_hcontracktree->Branch("_htrackcount",&_htrackcount,"htrackcount/I");
		_hcontracktree->Branch("_hlongesttrack",&_hlongesttrack,"hlongesttrack/D");
		delete _ncontracktree;
		_ncontracktree = new TTree("_ncontracktree","Largest Track Tree");
		_ncontracktree->Branch("_ntrackcount",&_ntrackcount,"ntrackcount/I");
		_ncontracktree->Branch("_nlongesttrack",&_nlongesttrack,"nlongesttrack/D");
	}


	bool AlgoNCPi0Finder::Reconstruct(const EventData &data, ParticleGraph& graph)
	{
	// Here we assume that we have run Algopi0
	// We will try not to use any informarion from primary finder 
	// --The idea is to tag ncpi0 events as primary  if they satisy the signal condiation of nc pi0 no pi+/-
		
	// First check if we have a pi0 
		int pi0count = 0;	
		std::vector<NodeID_t> pi0nodes;
		for ( auto const & p : graph.GetParticleArray()) if(p.PdgCode() == 111){ pi0count++; pi0nodes.push_back(p.ID());} 
			if(pi0count==0) return true;
		
	if(pi0count==1){// This is not the best way to do this all 
		for(unsigned int a =0; a<pi0nodes.size(); a++){
	// This should be a one time loop.... So really no need for it to be a loop 
	// Leave it this way for now ... we may want to loop over all pi0's 
			bool ncpi0 = true;
			// Get the pi0 particle 
			auto & pi0 = graph.GetParticle(pi0nodes[a]);
			auto pi0Vertex = pi0.Vertex();
			_pi0X = pi0Vertex[0];
			_pi0Y = pi0Vertex[1];
			_pi0Z = pi0Vertex[2];
			// Vertex of the pi0 point
			geoalgo::Point_t pi0vpt(pi0Vertex[0],pi0Vertex[1],pi0Vertex[2]);
			// _vtxsphere can be based on vertex error 
			auto vtxsphere = ::geoalgo::Sphere(pi0vpt, _vtxsphere_radius);
			// Now get the particle nodes for tracks 
			// get a vector of tracks
		// 1: both start and ends in sphere 
			std::vector<NodeID_t> Both_Tpts;	
		// 2: one point inside
			std::vector<NodeID_t> One_Tpts;	
		// 3: no points inside
			std::vector<NodeID_t> No_Tpts;	

		// Sort them out into catagories 
			for ( auto const& TrackNode : graph.GetParticleNodes(RecoType_t::kTrack) ){
				auto trackPart = graph.GetParticle(TrackNode);	
				auto const& Track = data.Track( graph.GetParticle(TrackNode).RecoID() );
				geoalgo::Point_t trk_start(Track.at(0)[0],Track.at(0)[1],Track.at(0)[2]);
				geoalgo::Point_t trk_end(Track.at(Track.size()-1)[0],Track.at(Track.size()-1)[1],Track.at(Track.size()-1)[2]);
				if(vtxsphere.Contain(trk_start) &&vtxsphere.Contain(trk_end) ) Both_Tpts.push_back(TrackNode);
				if((vtxsphere.Contain(trk_start) &&!vtxsphere.Contain(trk_end)) ||(!vtxsphere.Contain(trk_start) &&vtxsphere.Contain(trk_end))  ) One_Tpts.push_back(TrackNode);
				if(!vtxsphere.Contain(trk_start) && !vtxsphere.Contain(trk_end) ) No_Tpts.push_back(TrackNode);
			}

	// Look at the Case 2 tracks. that are half in and half out 
	// How many OPtracks are there? 
	_NTracks_OPTracks = One_Tpts.size();
			for ( auto const& onetrack : One_Tpts ){
		// Take the point that is not inside the sphere and project it out towards the box
				auto Otrackpart	= graph.GetParticle(onetrack);
				auto const& OTrack = data.Track(Otrackpart.RecoID());
				geoalgo::Point_t trk_start(OTrack.at(0)[0],OTrack.at(0)[1],OTrack.at(0)[2]);
				geoalgo::Point_t trk_end(OTrack.at(OTrack.size()-1)[0],OTrack.at(OTrack.size()-1)[1],OTrack.at(OTrack.size()-1)[2]);
				geoalgo::Point_t Out_point;
				geoalgo::Point_t In_point;
		// need to find out which point is out of the sphere
				if(vtxsphere.Contain(trk_start)){ Out_point = trk_end; In_point = trk_start;}
				else{Out_point = trk_start ;  In_point = trk_end;}
		// Then find the direction 
		// We will set the direction assuming that the point in the sphere is the start point
				geoalgo::Vector DirOutOfSphere(Out_point[0]-In_point[0],Out_point[1]-In_point[1],Out_point[2]-In_point[2]);
		//^^^^^ Does this need to be normalized?
				//std::cout<< "Dir of Sphere"<< DirOutOfSphere[0]<< " , "<< DirOutOfSphere[1]<< " , "<< DirOutOfSphere[2]<<std::endl;
		// Then get the intersect point to the TPC 
				::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);// What are the actual numbers? 
				auto ExitingPoint = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(In_point,DirOutOfSphere));
		// Then Get the distance from the point to the TPC 
				bool ptct = volume.Contain(In_point);
				if(!ptct) continue;  
				auto StartDistToEdge = In_point.Dist(ExitingPoint[0]);
				auto EndDistToEdge = Out_point.Dist(ExitingPoint[0]);
				// some useful couts
				std::cout<<" Track LEngth  "<<OTrack.Length()<<std::endl;
				std::cout<<" Star Dist To Edge " <<StartDistToEdge<<std::endl;
				std::cout<<" End Dist To Edge " <<EndDistToEdge<<std::endl;
				_Length_OPTracks = OTrack.Length();
				_SDistToTPC_OPTracks = StartDistToEdge;
				_EDistToTPC_OPTracks = EndDistToEdge;
				if(EndDistToEdge < 50.) ncpi0 = false;
				if(OTrack.Length()>50.) ncpi0 = false;

		// Fill the OP
		_OPtree->Fill();

		}// for loop over one pt tracks
		




	// Just look at first tracks
		_btrackcount = Both_Tpts.size();
		_blongesttrack = -999;
		double btrk = -999;
		for ( auto const& btrack : Both_Tpts ){
			auto Btrackpart	= graph.GetParticle(btrack);
			auto const& BTrack = data.Track(Btrackpart.RecoID());
//			if(BTrack.Length()>50) ncpi0 = false;
			if(BTrack.Length()>btrk){
				_blongesttrack = BTrack.Length();    
				btrk = BTrack.Length();
			}
		}


		// Just look at first tracks
		bool htrackcut = false;
		_htrackcount = One_Tpts.size();
		_hlongesttrack = -999;
		double htrk = -999;
		for ( auto const& onetrack : One_Tpts ){
			auto Otrackpart	= graph.GetParticle(onetrack);
			auto const& OTrack = data.Track(Otrackpart.RecoID());
			if(OTrack.Length()>htrk){
				_hlongesttrack = OTrack.Length();    
				htrk = OTrack.Length();
			}
		}

	// Just look at first tracks
		_ntrackcount = No_Tpts.size();
		double ntrk = -999;
		_nlongesttrack = -999;    
		for ( auto const& ntrack : No_Tpts ){
			auto Ntrackpart	= graph.GetParticle(ntrack);
			auto const& NTrack = data.Track(Ntrackpart.RecoID());
			if(NTrack.Length()>ntrk){
				_nlongesttrack = NTrack.Length();    
				ntrk = NTrack.Length();
			}
		}



		//_fulltracktree->Fill();
	//	_bcontracktree->Fill();
	//	_hcontracktree->Fill();
	//	_ncontracktree->Fill();


		
	// Cuts for returning if it is a NC event
//	if(_hlongesttrack<100.){
		if(ncpi0) graph.SetPrimary(pi0nodes[a]); 
		_fulltracktree->Fill();
		_bcontracktree->Fill();
		_hcontracktree->Fill();
		_ncontracktree->Fill();
//		}

		}// for loop over pi0nodes

	}// if 1 piPrimary0 is reco


return true;}

void AlgoNCPi0Finder::ProcessEnd(TFile* fout)
{
	if(fout){
		fout->cd();
		_OPtree->Write();
		_tracktree->Write();
		_fulltracktree->Write();
		_bcontracktree->Write();
		_hcontracktree->Write();
		_ncontracktree->Write();
	}
	
	else{
		delete _OPtree;
		delete _tracktree;
		delete _fulltracktree;
		delete _bcontracktree;
		delete _hcontracktree;
		delete _ncontracktree;
	}

	return;
}


// This might not needed
double AlgoNCPi0Finder::DistanceToTPC(::geoalgo::Point_t point , ::geoalgo::Vector direction ){
	double ret = 5.2;
	return ret;
}




}

#endif
