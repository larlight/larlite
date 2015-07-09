#ifndef ERTOOL_ALGOMICHELE_CXX
#define ERTOOL_ALGOMICHELE_CXX

#include "AlgoMichelE.h"

namespace ertool {

  size_t n_michel_e = 0;

  AlgoMichelE::AlgoMichelE(const std::string& name) : AlgoBase(name)
  {
    // histogram to hold the energy of each reconstructed michel electron
    michel_energy = 0;
    // histogram to hold the distance between the michel start point
    // and the muon track's end point
    shower_st_to_track_end = 0;

    // set default values for cut parameters
    _minMuonLength = 10;
    _maxDistance   = 3;

    // set verbosity to be off by default
    _verbose = false;
  }

  void AlgoMichelE::ProcessEnd(TFile* fout){
 
    if(fout){
      fout->cd();
      if(michel_energy)
	michel_energy->Write();
      if(shower_st_to_track_end)
	shower_st_to_track_end->Write();

    }

  }

  void AlgoMichelE::Finalize()
  {
    std::cout<<"Number of michel E's found is "<<n_michel_e<<std::endl;
  }

  void AlgoMichelE::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    InitializeHistos();

    return;
  }

  void AlgoMichelE::InitializeHistos(){
    
    if(!michel_energy)
      michel_energy = new TH1F("michel_energy","michel_energy",100,0,100);
    if(!shower_st_to_track_end)
      shower_st_to_track_end = new TH1F("shower_st_to_track_end","shower_st_to_track_end",100,0,100);
  }
  

  void AlgoMichelE::AcceptPSet(const ::fcllite::PSet& cfg) {
    
    // Load singleE params
    _alg_emp.AcceptPSet(cfg);
    
    return;
  }

  bool AlgoMichelE::Reconstruct(const EventData &data, ParticleGraph& graph)
  { 

    // Loop through Particles associated with a shower object
    for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){

      // get the shower object
      auto const& shower = data.Shower(graph.GetParticle(p).RecoID());

      //Ask EMParticle Algorithm if shower is electron, using only de/dx
      if( _alg_emp.LL(true, shower._dedx, -1) > _alg_emp.LL(false, shower._dedx, -1)){

	// Loop through Particles associated with a track
	for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
	  
	  // get track object
	  auto const& track = data.Track(graph.GetParticle(t).RecoID());
	  
	  // calculate distance between track end point and shower start point
	  auto const& trackEnd = track.back();
	  auto const shwrStart = shower.Start();
	  double dist = shwrStart.Dist(trackEnd);

	  // fill histogram with distance
	  shower_st_to_track_end->Fill(dist);

	  //Check if this shower is at the end of this tracks,
	  //Making sure the track is longer than the minimum length required
	  if( (shower.Start().Dist(track.back()) < _maxDistance) && 
	      (track.Length() > _minMuonLength) )
	    {
	      
	      // we found a new michel!
	      n_michel_e++;
	      
	      // edit the particle's information
	      graph.GetParticle(p).SetParticleInfo(11,
						   ParticleMass(11),
						   trackEnd,
						   shower.Dir()*shower._energy);

	      // also since we know it, add info on relationship
	      // between michel electron and muon track
	      // first: node of parent
	      // second: node of child
	      graph.SetParentage(t,p);

	      // finally, if in verbose mode, cout some info
	      if (_verbose){
		std::cout << "We found a michel electron!" << std::endl
			  << "Track end point: " << trackEnd << std::endl
			  << "Shower start point: " << shwrStart << std::endl
			  << "Shower energy: " << shower._energy << " [MeV]" << std::endl
			  << std::endl;
	      }
	      
	      // fill histogram with michel electron energy
	      michel_energy->Fill(shower._energy);
	      
	      // we found that this shower is a michel -> stop looping through tracks
	      break;
	      
	    }//End if michel was found
	  
	}//End loop over tracks
      }//End if shower is likely an electron
    }//End loop over showers
    
    return true;
    
  }
  
}

#endif
